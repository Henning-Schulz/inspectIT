#pragma once

#include "BufferStrategy.h"
#include "Logger.h"

#include <mutex>

namespace inspectit {
	namespace sending {
		namespace buffer {

			class SimpleBufferStrategy : public BufferStrategy
			{
			private:
				std::vector<std::shared_ptr<inspectit::data::SensorData>> measurements;
				bool newMeasurements = false;
				std::mutex measurementsMutex;

				inspectit::logger::Logger logger = loggerFactory.createLogger("SimpleBufferStrategy");

			public:
				SimpleBufferStrategy();
				~SimpleBufferStrategy();

				void addMeasurements(std::vector<std::shared_ptr<inspectit::data::SensorData>> data);
				bool hasNext();
				std::vector<std::shared_ptr<inspectit::data::SensorData>> next();
			};

		}
	}
}