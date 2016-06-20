#include "DataSendingService.h"


DataSendingService *dataSendingService;

void startDataSendingService(std::shared_ptr<BufferStrategy> bufferStrategy, std::shared_ptr<SendingStrategy> sendingStrategy)
{
	dataSendingService = new DataSendingService(bufferStrategy, sendingStrategy);
}

void shutdownDataSendingService()
{
	delete dataSendingService;
}

DataSendingService::DataSendingService(std::shared_ptr<BufferStrategy> bufferStrategy, std::shared_ptr<SendingStrategy> sendingStrategy)
{
	this->bufferStrategy = bufferStrategy;
	this->sendingStrategy = sendingStrategy;
	sendingStrategy->start();

	measurements = std::make_shared<std::map<std::string, std::shared_ptr<MethodSensorData>>>();
	measurementsPrepCopy = std::make_shared<std::map<std::string, std::shared_ptr<MethodSensorData>>>();

	storages = std::make_shared<std::map<std::string, std::shared_ptr<MeasurementStorage>>>();
	storagesPrepCopy = std::make_shared<std::map<std::string, std::shared_ptr<MeasurementStorage>>>();

	finishedStorages = std::make_shared<std::vector<std::shared_ptr<MeasurementStorage>>>();
	finishedStoragesPrepCopy = std::make_shared<std::vector<std::shared_ptr<MeasurementStorage>>>();

	DataSendingService *that = this;

	preparingThread = std::thread([that]() { that->prepareData(); });
	sendingThread = std::thread([that]() { that->sendData(); });

	logger.info("Data sending service created.");
}


DataSendingService::~DataSendingService()
{
	stop = true;
	sendingStrategy->stop();

	cvPrepare.notify_all();
	preparingThread.join();
	cvSend.notify_all();
	sendingThread.join();

	logger.info("All measurement data sent.");
}

std::shared_ptr<MethodSensorData> DataSendingService::getMethodSensorData(JAVA_LONG sensorTypeId, JAVA_LONG methodId, std::string prefix)
{
	std::shared_lock<std::shared_mutex> lock(mMeasurements);
	auto it = measurements->find(createKey(sensorTypeId, methodId, prefix));

	if (it == measurements->end()) {
		return nullptr;
	} else {
		return it->second;
	}
}

void DataSendingService::addMethodSensorData(std::shared_ptr<MethodSensorData> data, std::string prefix)
{
	std::unique_lock<std::shared_mutex> lock(mMeasurements);
	measurements->insert({ createKey(data->getMethodSensorId(), data->getMethodId(), prefix), data });
	notifiyListSizeListeners();
}

std::shared_ptr<MeasurementStorage> DataSendingService::getMeasurementStorage(JAVA_LONG sensorTypeId, JAVA_LONG methodId, std::string prefix)
{
	std::shared_lock<std::shared_mutex> lock(mStorages);
	auto it = storages->find(createKey(sensorTypeId, methodId, prefix));

	if (it == storages->end()) {
		return nullptr;
	} else {
		return it->second;
	}
}

void DataSendingService::addMeasurementStorage(std::shared_ptr<MeasurementStorage> storage, std::string prefix)
{
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
		}
	}

	storages->insert({ key, storage });
	notifiyListSizeListeners();
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

		if (!measurements->empty() || !storages->empty())
		{
			logger.debug("Preparing data...");

			{
				std::unique_lock<std::shared_mutex> lock(mMeasurements);
				std::swap(measurements, measurementsPrepCopy);
			}

			{
				std::unique_lock<std::shared_mutex> lock(mStorages);
				std::swap(storages, storagesPrepCopy);
			}

			{
				std::unique_lock<std::shared_mutex> lock(mFinishedStorages);
				std::swap(finishedStorages, finishedStoragesPrepCopy);
			}

			logger.debug("Maps swapped");

			std::vector<std::shared_ptr<MethodSensorData>> dataObjects;
			for (auto it = measurementsPrepCopy->begin(); it != measurementsPrepCopy->end(); it++) {
				dataObjects.push_back(it->second);
			}

			measurementsPrepCopy->clear();

			for (auto it = finishedStoragesPrepCopy->begin(); it != finishedStoragesPrepCopy->end(); it++) {
				std::shared_ptr<MeasurementStorage> storage = *it;
				if (storage->finished()) {
					dataObjects.push_back(storage->finalizeData());
				}
				else {
					logger.warn("Measurement storage for sensor %lli and method %i was not yet finished. Data are lost!", storage->getMethodSensorId(), storage->getMethodId());
				}
			}

			finishedStorages->clear();

			for (auto it = storagesPrepCopy->begin(); it != storagesPrepCopy->end(); ) {
				if (it->second->finished()) {
					logger.debug("Data finished. Preparing...");
					dataObjects.push_back(it->second->finalizeData());
					it = storagesPrepCopy->erase(it);
				}
				else {
					++it;
				}
			}

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
			std::vector<std::shared_ptr<MethodSensorData>> dataToSend = bufferStrategy->next();
			cmrConnection->sendDataObjects(dataToSend, stop); // wait for request only during shutdown
		}
	}

	logger.debug("Sending thread stopped.");
}

void DataSendingService::notifiyListSizeListeners()
{
	for (auto it = listSizeListeners.begin(); it != listSizeListeners.end(); it++) {
		(*it)->listSizeChanged(measurements->size());
	}
}

void DataSendingService::addListSizeListener(std::shared_ptr<ListSizeListener> listener)
{
	listSizeListeners.push_back(listener);
}