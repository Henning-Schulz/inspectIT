#pragma once

#include "MeasurementStorage.h"
#include "CMRConnection.h"
#include "RingBuffer.h"
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

		class DataSendingService
		{
		private:
			inspectit::sending::RingBuffer ringBuffer;
			std::vector<std::shared_ptr<inspectit::data::SensorData>> sendingBuffer;
			std::vector<int> indexBuffer;

			std::thread sendingThread;

			bool stop = false;

			inspectit::logger::Logger logger = loggerFactory.createLogger("DataSendingService");

			void sendData();

		public:
			DataSendingService(size_t bufferSize);
			~DataSendingService();

			std::shared_ptr<inspectit::sending::SensorDataWrapper> getDataWrapper();
			void publishDataWrapper(std::shared_ptr<inspectit::sending::SensorDataWrapper> wrapper);

			void stopSending();
		};

	}
}