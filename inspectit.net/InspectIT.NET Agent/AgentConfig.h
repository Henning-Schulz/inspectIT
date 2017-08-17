#pragma once

#include "basehdr.h"
#include "JsonReceivable.h"
#include "PlatformSensorTypeConfig.h"
#include "MethodSensorTypeConfig.h"
#include "ExceptionSensorTypeConfig.h"
#include "StrategyConfig.h"
#include "InstrumentationDefinition.h"
#include "Logger.h"

#include <map>

namespace inspectit {
	namespace config {

		class AgentConfig : inspectit::json::JsonReceivable
		{
		private:
			JAVA_LONG platformId;
			bool classCacheExistsOnCmr;

			std::vector<std::shared_ptr<PlatformSensorTypeConfig>> platformSensorTypeConfigs;
			std::vector<std::shared_ptr<MethodSensorTypeConfig>> methodSensorTypeConfigs;
			std::shared_ptr<ExceptionSensorTypeConfig> exceptionSensorTypeConfig;
			// JmxSensorTypeConfig does not make sense here
			std::vector<std::shared_ptr<MethodSensorTypeConfig>> specialMethodSensorTypeConfigs;

			std::shared_ptr<StrategyConfig> bufferSizeConfig;

			std::vector<std::wstring> excludeClassesPatterns;
			std::map<std::vector<std::wstring>, std::shared_ptr<InstrumentationDefinition>> initialInstrumentationDefinitions;

			std::wstring configurationInfo;

			inspectit::logger::Logger logger = loggerFactory.createLogger("AgentConfig");

		public:
			AgentConfig();
			~AgentConfig();

			JAVA_LONG getPlatformId();
			bool doesClassCacheExistOnCmr();

			std::vector<std::shared_ptr<PlatformSensorTypeConfig>> getPlatformSensorTypeConfigs();
			std::vector<std::shared_ptr<MethodSensorTypeConfig>> getMethodSensorTypeConfigs();
			std::shared_ptr<ExceptionSensorTypeConfig> getExceptionSensorTypeConfig();
			// JmxSensorTypeConfig does not make sense here
			std::vector<std::shared_ptr<MethodSensorTypeConfig>> getSpecialMethodSensorTypeConfigs();

			std::shared_ptr<StrategyConfig> getBufferSizeConfig();

			std::vector<std::wstring> getExcludeClassesPatterns();
			std::map<std::vector<std::wstring>, std::shared_ptr<InstrumentationDefinition>> getInitialInstrumentationDefinitions();

			std::wstring getConfigurationInfo();

			void fromJson(web::json::object json);
		};

	}
}