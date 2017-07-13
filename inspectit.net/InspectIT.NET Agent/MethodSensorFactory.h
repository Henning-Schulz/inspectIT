#pragma once

#include "MethodSensor.h"
#include "MethodSensorTypeConfig.h"
#include "Logger.h"

#include <shared_mutex>

namespace inspectit {
	namespace instrumentation {

		class MethodSensorFactory
		{
		private:
			std::map<std::wstring, std::shared_ptr<inspectit::sensor::MethodSensor>> knownSensors;

			void setupKnownSensors();
			void addKnownSensor(std::shared_ptr<inspectit::sensor::MethodSensor> sensor);

			inspectit::logger::Logger logger = loggerFactory.createLogger("MethodSensorFactory");

		public:
			MethodSensorFactory();
			~MethodSensorFactory();

			std::shared_ptr<inspectit::sensor::MethodSensor> createMethodSensor(std::shared_ptr<inspectit::config::MethodSensorTypeConfig> sensorTypeConfig, JAVA_LONG platformId, ICorProfilerInfo3 *profilerInfo, std::shared_ptr<inspectit::sending::DataSendingService> dataSendingService);
		};

	}
}