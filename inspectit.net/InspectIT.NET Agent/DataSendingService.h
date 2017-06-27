#pragma once

#include "BufferStrategy.h"
#include "SendingStrategy.h"
#include "ListSizeListener.h"
#include "MeasurementStorage.h"
#include "CMRConnection.h"
#include "Logger.h"

#include <map>
#include <string>
#include <sstream>

#include <memory>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <shared_mutex>

class DataSendingService : public ListSizeListenable
{
private:
	std::shared_ptr<BufferStrategy> bufferStrategy;
	std::shared_ptr<SendingStrategy> sendingStrategy;
	std::unique_ptr<std::map<std::string, std::shared_ptr<SensorData>>> measurements;
	std::unique_ptr<std::map<std::string, std::shared_ptr<SensorData>>> measurementsPrepCopy;
	std::shared_mutex mMeasurements;

	std::unique_ptr<std::map<std::string, std::shared_ptr<MeasurementStorage>>> storages;
	std::shared_mutex mStorages;

	std::unique_ptr<std::vector<std::shared_ptr<MeasurementStorage>>> finishedStorages;
	std::unique_ptr<std::vector<std::shared_ptr<MeasurementStorage>>> finishedStoragesPrepCopy;
	std::shared_mutex mFinishedStorages;

	std::thread preparingThread;
	std::thread sendingThread;
	std::mutex mPrepare;
	std::mutex mSend;
	std::condition_variable cvPrepare;
	std::condition_variable cvSend;

	bool stop = false;

	Logger logger = loggerFactory.createLogger("DataSendingService");

	std::vector<std::shared_ptr<ListSizeListener>> listSizeListeners;

	void prepareData();
	void sendData();

	std::string createKey(JAVA_LONG sensorTypeId, JAVA_LONG methodId, std::string prefix);

	void notifiyListSizeListeners();

public:
	DataSendingService(std::shared_ptr<BufferStrategy> bufferStrategy, std::shared_ptr<SendingStrategy> sendingStrategy);
	~DataSendingService();

	std::shared_ptr<SensorData> getMethodSensorData(JAVA_LONG sensorTypeId, JAVA_LONG methodId, std::string prefix = "");
	void addMethodSensorData(std::shared_ptr<SensorData> data, std::string prefix = "");

	std::shared_ptr<MeasurementStorage> getMeasurementStorage(JAVA_LONG sensorTypeId, JAVA_LONG methodId, std::string prefix = "");
	void addMeasurementStorage(std::shared_ptr<MeasurementStorage> storage, std::string prefix = "");
	void notifyStorageFinished(std::shared_ptr<MeasurementStorage> storage, std::string prefix = "");

	void send();

	void addListSizeListener(std::shared_ptr<ListSizeListener> listener);
};

extern DataSendingService *dataSendingService;

void startDataSendingService(std::shared_ptr<BufferStrategy> bufferStrategy, std::shared_ptr<SendingStrategy> sendingStrategy);
void shutdownDataSendingService();