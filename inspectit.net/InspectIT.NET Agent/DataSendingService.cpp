#include "DataSendingService.h"

using namespace inspectit::config;
using namespace inspectit::data;
using namespace inspectit::sending;
using namespace inspectit::storage;

DataSendingService::DataSendingService(size_t bufferSize)
{
	this->ringBuffer.initialize(bufferSize);
	this->sendingBuffer.reserve(bufferSize);
	this->indexBuffer.reserve(bufferSize);

	DataSendingService *that = this;

	sendingThread = std::thread([that]() { that->sendData(); });

	logger.info("Data sending service created.");
}


DataSendingService::~DataSendingService()
{
	stopSending();
}

std::shared_ptr<inspectit::sending::SensorDataWrapper> inspectit::sending::DataSendingService::getDataWrapper()
{
	int slot = ringBuffer.tryNext();

	if (slot > 0) {
		return ringBuffer.get(slot);
	}
	else {
		return std::shared_ptr<SensorDataWrapper>();
	}
}

void inspectit::sending::DataSendingService::publishDataWrapper(std::shared_ptr<inspectit::sending::SensorDataWrapper> wrapper)
{
	ringBuffer.publish(wrapper->getSlot());
}

void DataSendingService::sendData()
{
	while (!stop)
	{
		ringBuffer.waitForData(); // Only wait if there are no new data

		sendingBuffer.clear();
		indexBuffer.clear();
		auto iterator = ringBuffer.iterator();

		int next;
		while (iterator.hasNext()) {
			next = iterator.next();
			sendingBuffer.push_back(ringBuffer.get(next)->getData());
			indexBuffer.push_back(next);
		}

		logger.debug("Sending %i data objects...", sendingBuffer.size());
		cmrConnection->sendDataObjects(sendingBuffer, true); // wait for the CMR processing the request
		logger.debug("Data sent");

		for (auto it = indexBuffer.begin(); it != indexBuffer.end(); it++) {
			ringBuffer.free(*it);
		}
	}

	logger.debug("Sending thread stopped.");
}

void DataSendingService::stopSending()
{
	logger.debug("Stop sending...");
	if (!stop) {
		logger.debug("Really stop sending...");
		stop = true;
		sendingThread.join();
		logger.info("All measurement data sent.");
	}
}
