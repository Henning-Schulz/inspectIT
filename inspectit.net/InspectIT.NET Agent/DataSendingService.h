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

namespace inspectit {
	namespace sending {

		class DataSendingService : public strategy::ListSizeListenable
		{
		private:
			std::shared_ptr<buffer::BufferStrategy> bufferStrategy;
			std::shared_ptr<strategy::SendingStrategy> sendingStrategy;
			std::unique_ptr<std::map<std::string, std::shared_ptr<inspectit::data::SensorData>>> measurements;
			std::unique_ptr<std::map<std::string, std::shared_ptr<inspectit::data::SensorData>>> measurementsPrepCopy;
			std::shared_mutex mMeasurements;

			std::unique_ptr<std::map<std::string, std::shared_ptr<inspectit::storage::MeasurementStorage>>> storages;
			std::shared_mutex mStorages;

			std::unique_ptr<std::vector<std::shared_ptr<inspectit::storage::MeasurementStorage>>> finishedStorages;
			std::unique_ptr<std::vector<std::shared_ptr<inspectit::storage::MeasurementStorage>>> finishedStoragesPrepCopy;
			std::shared_mutex mFinishedStorages;

			std::thread preparingThread;
			std::thread sendingThread;
			std::mutex mPrepare;
			std::mutex mSend;
			std::condition_variable cvPrepare;
			std::condition_variable cvSend;

			bool stop = false;

			inspectit::logger::Logger logger = loggerFactory.createLogger("DataSendingService");

			std::vector<std::shared_ptr<strategy::ListSizeListener>> listSizeListeners;

			void prepareData();
			void sendData();

			std::string createKey(JAVA_LONG sensorTypeId, JAVA_LONG methodId, std::string prefix);

			std::shared_ptr<buffer::BufferStrategy> createBufferStrategy(std::shared_ptr<inspectit::config::StrategyConfig> config);
			std::shared_ptr<strategy::SendingStrategy> createSendingStrategy(std::shared_ptr<inspectit::config::StrategyConfig> config);

			void notifiyListSizeListeners();

		public:
			DataSendingService(std::shared_ptr<inspectit::config::StrategyConfig> bufferStrategyConfig, std::shared_ptr<inspectit::config::StrategyConfig> sendingStrategyConfig);
			~DataSendingService();

			std::shared_ptr<inspectit::data::SensorData> getMethodSensorData(JAVA_LONG sensorTypeId, JAVA_LONG methodId, std::string prefix = "");
			void addMethodSensorData(std::shared_ptr<inspectit::data::SensorData> data, std::string prefix = "");

			std::shared_ptr<inspectit::storage::MeasurementStorage> getMeasurementStorage(JAVA_LONG sensorTypeId, JAVA_LONG methodId, std::string prefix = "");
			void addMeasurementStorage(std::shared_ptr<inspectit::storage::MeasurementStorage> storage, std::string prefix = "");
			void notifyStorageFinished(std::shared_ptr<inspectit::storage::MeasurementStorage> storage, std::string prefix = "");

			void send();

			void addListSizeListener(std::shared_ptr<strategy::ListSizeListener> listener);

			void stopSending();
		};

	}
}