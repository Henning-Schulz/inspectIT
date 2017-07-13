#pragma once

#include "basehdr.h"
#include "SensorInstrumentationPoint.h"
#include "ClassType.h"
#include "InstrumentationDefinition.h"
#include "MethodSensor.h"
#include "MethodSensorFactory.h"
#include "DataSendingService.h"
#include "Logger.h"

#include <map>
#include <vector>
#include <string>

namespace inspectit {
	namespace instrumentation {

		class InstrumentationManager
		{
		private:
			JAVA_LONG platformId;
			ICorProfilerInfo3 *profilerInfo;

			std::vector<std::wstring> excludeClassesPatterns;
			std::map<mdMethodDef, std::shared_ptr<inspectit::config::SensorInstrumentationPoint>> sensorInstrumentationPoints;

			std::map<JAVA_LONG, std::shared_ptr<inspectit::config::MethodSensorTypeConfig>> sensorTypeConfigs;
			std::map<JAVA_LONG, std::shared_ptr<inspectit::sensor::MethodSensor>> methodSensors;

			MethodSensorFactory methodSensorFactory;

			std::shared_ptr<inspectit::sending::DataSendingService> dataSendingService;

			inspectit::logger::Logger logger = loggerFactory.createLogger("InstrumentationManager");

		public:
			InstrumentationManager(JAVA_LONG platformId, ICorProfilerInfo3 *profilerInfo, std::shared_ptr<inspectit::sending::DataSendingService> dataSendingService);
			~InstrumentationManager();

			void addSensorTypeConfigs(std::vector<std::shared_ptr<inspectit::config::MethodSensorTypeConfig>> configs);

			void setExcludeClassesPatterns(std::vector<std::wstring> excludeClassesPatterns);
			bool isExcluded(std::shared_ptr<inspectit::types::ClassType> classType);
			bool instrumentationDefinitionMatches(std::shared_ptr<inspectit::types::MethodType> methodType, std::shared_ptr<inspectit::config::MethodInstrumentationConfig> instrConfig);

			void registerInstrumentationDefinition(std::shared_ptr<inspectit::types::ClassType> classType, std::shared_ptr<inspectit::config::InstrumentationDefinition> instrDef);
			std::shared_ptr<inspectit::config::SensorInstrumentationPoint> getSensorInstrumentationPoint(mdMethodDef methodDef);

			std::shared_ptr<inspectit::sensor::MethodSensor> getMethodSensorForId(JAVA_LONG sensorId);
		};

	}
}