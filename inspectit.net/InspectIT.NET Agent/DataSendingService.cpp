#include "DataSendingService.h"
#include "SimpleBufferStrategy.h"
#include "DroppingBufferStrategy.h"
#include "ListSizeStrategy.h"


DataSendingService::DataSendingService(std::shared_ptr<StrategyConfig> bufferStrategyConfig, std::shared_ptr<StrategyConfig> sendingStrategyConfig)
{
	this->bufferStrategy = createBufferStrategy(bufferStrategyConfig);
	this->sendingStrategy = createSendingStrategy(sendingStrategyConfig);
	sendingStrategy->start();

	measurements = std::make_unique<std::map<std::string, std::shared_ptr<SensorData>>>();
	measurementsPrepCopy = std::make_unique<std::map<std::string, std::shared_ptr<SensorData>>>();

	storages = std::make_unique<std::map<std::string, std::shared_ptr<MeasurementStorage>>>();

	finishedStorages = std::make_unique<std::vector<std::shared_ptr<MeasurementStorage>>>();
	finishedStoragesPrepCopy = std::make_unique<std::vector<std::shared_ptr<MeasurementStorage>>>();

	DataSendingService *that = this;

	preparingThread = std::thread([that]() { that->prepareData(); });
	sendingThread = std::thread([that]() { that->sendData(); });

	logger.info("Data sending service created.");
}


DataSendingService::~DataSendingService()
{
	stopSending();
}

std::shared_ptr<BufferStrategy> DataSendingService::createBufferStrategy(std::shared_ptr<StrategyConfig> config)
{
	if (config->getClassName().compare(L"rocks.inspectit.agent.java.buffer.impl.SimpleBufferStrategy") == 0) {
		return std::make_shared<SimpleBufferStrategy>();
	}
	else {
		logger.error("Buffer strategy %ls is not supported! Data will be dropped.", config->getClassName().c_str());
		return std::make_shared<DroppingBufferStrategy>();
	}
}

std::shared_ptr<SendingStrategy> DataSendingService::createSendingStrategy(std::shared_ptr<StrategyConfig> config)
{
	if (config->getClassName().compare(L"rocks.inspectit.agent.java.sending.impl.ListSizeStrategy") == 0) {
		size_t maxSize = _wtoi(config->getSettings().at(L"size").c_str());
		return std::make_shared<ListSizeStrategy>(maxSize);
	}
	else {
		logger.error("Sending strategy %ls is not supported! No data will be sent.", config->getClassName().c_str());
	}
}

std::shared_ptr<SensorData> DataSendingService::getMethodSensorData(JAVA_LONG sensorTypeId, JAVA_LONG methodId, std::string prefix)
{
	std::shared_lock<std::shared_mutex> lock(mMeasurements);
	auto it = measurements->find(createKey(sensorTypeId, methodId, prefix));

	if (it == measurements->end()) {
		return nullptr;
	} else {
		return it->second;
	}
}

void DataSendingService::addMethodSensorData(std::shared_ptr<SensorData> data, std::string prefix)
{
	std::unique_lock<std::shared_mutex> lock(mMeasurements);
	measurements->insert({ createKey(data->getSensorTypeId(), data->getIdentifyingId(), prefix), data });
	notifiyListSizeListeners();
}

std::shared_ptr<MeasurementStorage> DataSendingService::getMeasurementStorage(JAVA_LONG sensorTypeId, JAVA_LONG methodId, std::string prefix)
{
	std::shared_lock<std::shared_mutex> shared_lock(mStorages);
	auto it = storages->find(createKey(sensorTypeId, methodId, prefix));

	if (it == storages->end() || it->second->finished()) {
		return nullptr;
	}
	else if (it->second->finished()) {
		logger.debug("Measurement storage for method %i is already finished. Returning nullptr.", methodId);
		return nullptr;
	} 
	else {
		return it->second;
	}
}

void DataSendingService::addMeasurementStorage(std::shared_ptr<MeasurementStorage> storage, std::string prefix)
{
	if (storage->finished()) {
		std::unique_lock<std::shared_mutex> uniqueLock(mFinishedStorages);
		finishedStorages->push_back(storage);
		notifiyListSizeListeners();
	}
	else {
		std::unique_lock<std::shared_mutex> lock(mStorages);

		std::string key = createKey(storage->getMethodSensorId(), storage->getMethodId(), prefix);

		{
			// If a storage with the specified key already exists,
			// it is moved to the finishedStorages list
			std::unique_lock<std::shared_mutex> uniqueLock(mFinishedStorages);
			auto it = storages->find(key);
			if (it != storages->end()) {
				finishedStorages->push_back(it->second);
				storages->erase(key);
				notifiyListSizeListeners();
			}
		}

		storages->insert({ key, storage });
	}
}

void DataSendingService::notifyStorageFinished(std::shared_ptr<MeasurementStorage> storage, std::string prefix)
{
	//logger.debug("notifyStorageFinished");

	if (!storage->finished()) {
		logger.error("Storage for sensor %lli, method %lli and prefix %s is not finished!", storage->getMethodSensorId(), storage->getMethodId(), prefix.c_str());
		return;
	}

	std::unique_lock<std::shared_mutex> lock(mStorages);

	std::string key = createKey(storage->getMethodSensorId(), storage->getMethodId(), prefix);

	{
		std::unique_lock<std::shared_mutex> uniqueLock(mFinishedStorages);
		auto it = storages->find(key);
		if (it != storages->end()) {
			finishedStorages->push_back(it->second);
			storages->erase(it);
			notifiyListSizeListeners();
		}
		else {
			logger.error("Storage for sensor %lli, method %lli and prefix %s is not contained in the saved storages!", storage->getMethodSensorId(), storage->getMethodId(), prefix.c_str());
		}
	}
}

std::string DataSendingService::createKey(JAVA_LONG sensorTypeId, JAVA_LONG methodId, std::string prefix)
{
	std::stringstream ss;

	if (!prefix.empty()) {
		ss << prefix << ".";
	}

	ss << methodId << "." << sensorTypeId;

	return ss.str();
}

void DataSendingService::send()
{
	std::unique_lock<std::mutex> lock(mPrepare);
	cvPrepare.notify_all();
}

void DataSendingService::prepareData()
{
	while (!stop)
	{
		{
			std::unique_lock<std::mutex> lock(mPrepare);
			cvPrepare.wait(lock);
		}

		bool empty = true;

		{
			std::shared_lock<std::shared_mutex> measurementsLock(mMeasurements);
			empty = measurements->empty();
		}

		if (empty) {
			std::shared_lock<std::shared_mutex> storagesLock(mFinishedStorages);
			empty = finishedStorages->empty();
		}

		if (!empty)
		{
			logger.debug("Preparing data...");

			{
				std::unique_lock<std::shared_mutex> lock(mMeasurements);
				std::swap(measurements, measurementsPrepCopy);
			}

			{
				std::unique_lock<std::shared_mutex> lock(mFinishedStorages);
				std::swap(finishedStorages, finishedStoragesPrepCopy);
			}

			logger.debug("Maps swapped");
			logger.debug("finishedStoragesPrepCopy has size %i", finishedStoragesPrepCopy->size());

			std::vector<std::shared_ptr<SensorData>> dataObjects;
			for (auto it = measurementsPrepCopy->begin(); it != measurementsPrepCopy->end(); it++) {
				dataObjects.push_back(it->second);
			}

			measurementsPrepCopy->clear();

			for (auto it = finishedStoragesPrepCopy->begin(); it != finishedStoragesPrepCopy->end(); it++) {
				std::shared_ptr<MeasurementStorage> storage = *it;
				if (storage->finished()) {
					auto finalized = storage->finalizeData();
					for (auto finIt = finalized.begin(); finIt != finalized.end(); finIt++) {
						dataObjects.push_back(*finIt);
					}
				}
				else {
					logger.warn("Measurement storage for sensor %lli and method %i was not yet finished. Data is lost!", storage->getMethodSensorId(), storage->getMethodId());
				}
			}

			finishedStoragesPrepCopy->clear();

			logger.debug("dataObjects created");

			bufferStrategy->addMeasurements(dataObjects);

			logger.debug("Added to buffer");

			{
				std::unique_lock<std::mutex> lock(mSend);
				cvSend.notify_all();
			}

			logger.debug("Sender notified");
		}
	}

	logger.debug("Preparing thread stopped.");
}

void DataSendingService::sendData()
{
	while (!stop)
	{
		if (!bufferStrategy->hasNext())
		{
			{
				std::unique_lock<std::mutex> lock(mSend);
				cvSend.wait(lock);
			}
		}

		logger.debug("Sending data...");

		while (bufferStrategy->hasNext()) {
			std::vector<std::shared_ptr<SensorData>> dataToSend = bufferStrategy->next();
			cmrConnection->sendDataObjects(dataToSend, stop); // wait for request only during shutdown
		}
	}

	logger.debug("Sending thread stopped.");
}

void DataSendingService::notifiyListSizeListeners()
{
	for (auto it = listSizeListeners.begin(); it != listSizeListeners.end(); it++) {
		logger.debug("List size is %i + %i = %i", measurements->size(), finishedStorages->size(), measurements->size() + finishedStorages->size());
		// Pass the summarized sizes to the listeners.
		(*it)->listSizeChanged(measurements->size() + finishedStorages->size());
	}
}

void DataSendingService::addListSizeListener(std::shared_ptr<ListSizeListener> listener)
{
	listSizeListeners.push_back(listener);
}

void DataSendingService::stopSending()
{
	logger.debug("Stop sending...");
	if (!stop) {
		logger.debug("Really stop sending...");
		stop = true;
		sendingStrategy->stop();

		cvPrepare.notify_all();
		preparingThread.join();
		cvSend.notify_all();
		sendingThread.join();

		logger.info("All measurement data sent.");
	}
}
