#include "AgentConfig.h"



AgentConfig::AgentConfig()
{
}


AgentConfig::~AgentConfig()
{
}

JAVA_LONG AgentConfig::getPlatformId()
{
	return platformId;
}

bool AgentConfig::doesClassCacheExistOnCmr()
{
	return classCacheExistsOnCmr;
}

std::vector<std::shared_ptr<PlatformSensorTypeConfig>> AgentConfig::getPlatformSensorTypeConfigs()
{
	return platformSensorTypeConfigs;
}

std::vector<std::shared_ptr<MethodSensorTypeConfig>> AgentConfig::getMethodSensorTypeConfigs()
{
	return methodSensorTypeConfigs;
}

std::shared_ptr<ExceptionSensorTypeConfig> AgentConfig::getExceptionSensorTypeConfig()
{
	return exceptionSensorTypeConfig;
}

std::vector<std::shared_ptr<MethodSensorTypeConfig>> AgentConfig::getSpecialMethodSensorTypeConfigs()
{
	return specialMethodSensorTypeConfigs;
}

std::shared_ptr<StrategyConfig> AgentConfig::getBufferStrategyConfig()
{
	return bufferStrategyConfig;
}

std::shared_ptr<StrategyConfig> AgentConfig::getSendingStrategyConfig()
{
	return sendingStrategyConfig;
}

std::vector<std::wstring> AgentConfig::getExcludeClassesPatterns()
{
	return excludeClassesPatterns;
}

std::map<std::vector<std::wstring>, std::shared_ptr<InstrumentationDefinition>> AgentConfig::getInitialInstrumentationDefinitions()
{
	return initialInstrumentationDefinitions;
}

std::wstring AgentConfig::getConfigurationInfo()
{
	return configurationInfo;
}

void AgentConfig::fromJson(json::object json)
{
	logger.debug("Creating from JSON...");

	// general settings
	platformId = json.at(L"platformId").as_number().to_int64();
	classCacheExistsOnCmr = json.at(L"classCacheExistsOnCmr").as_bool();

	logger.debug("1");

	// sensor type configs
	json::array configArray = json.at(L"platformSensorTypeConfigs").as_array();
	for (auto it = configArray.begin(); it != configArray.end(); it++) {
		auto platformSensorConfig = std::make_shared<PlatformSensorTypeConfig>();
		platformSensorConfig->fromJson(it->as_object());
		platformSensorTypeConfigs.push_back(platformSensorConfig);
	}

	logger.debug("2");

	configArray = json.at(L"methodSensorTypeConfigs").as_array();
	logger.debug("configArray: %ls", json.at(L"methodSensorTypeConfigs").serialize().c_str());
	logger.debug("2.1");
	for (auto it = configArray.begin(); it != configArray.end(); it++) {
		logger.debug("2.2");
		auto methodSensorConfig = std::make_shared<MethodSensorTypeConfig>();
		logger.debug("2.3");
		methodSensorConfig->fromJson(it->as_object());
		logger.debug("2.4");
		methodSensorTypeConfigs.push_back(methodSensorConfig);
		logger.debug("2.5");
	}

	logger.debug("3");

	exceptionSensorTypeConfig = std::make_shared<ExceptionSensorTypeConfig>();
	exceptionSensorTypeConfig->fromJson(json.at(L"exceptionSensorTypeConfig").as_object());

	configArray = json.at(L"methodSensorTypeConfigs").as_array();
	for (auto it = configArray.begin(); it != configArray.end(); it++) {
		auto methodSensorConfig = std::make_shared<MethodSensorTypeConfig>();
		methodSensorConfig->fromJson(it->as_object());
		specialMethodSensorTypeConfigs.push_back(methodSensorConfig);
	}

	logger.debug("4");

	// strategy configs
	bufferStrategyConfig = std::make_shared<StrategyConfig>();
	bufferStrategyConfig->fromJson(json.at(L"bufferStrategyConfig").as_object());

	sendingStrategyConfig = std::make_shared<StrategyConfig>();
	sendingStrategyConfig->fromJson(json.at(L"sendingStrategyConfig").as_object());

	logger.debug("5");

	// exclusions
	json::array excludesArray = json.at(L"excludeClassesPatterns").as_array();
	for (auto it = excludesArray.begin(); it != excludesArray.end(); it++) {
		std::wstring pattern = it->as_object().at(L"pattern").as_string();
		excludeClassesPatterns.push_back(pattern);
	}

	logger.debug("6");

	// initial instr results
	json::object instrResultsMap = json.at(L"initialInstrumentationResults").as_object();
	for (auto it = instrResultsMap.begin(); it != instrResultsMap.end(); it++) {
		auto instrumentationDef = std::make_shared<InstrumentationDefinition>();
		instrumentationDef->fromJson(it->second.as_object());

		json::value keyListJsonString(it->first);
		json::array keyJsonList = keyListJsonString.as_array();
		std::vector<std::wstring> keyList;

		for (auto keyIt = keyJsonList.begin(); keyIt != keyJsonList.end(); keyIt++) {
			keyList.push_back(keyIt->as_string());
		}

		initialInstrumentationDefinitions.emplace(keyList, instrumentationDef);
	}

	logger.debug("7");

	// configuration info
	configurationInfo = json.at(L"configurationInfo").as_string();

	logger.debug("8");
}
