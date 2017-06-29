#include "AgentConfig.h"

#include <sstream>
#include <iostream>



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
	// general settings
	platformId = json.at(L"platformId").as_number().to_int64();
	classCacheExistsOnCmr = json.at(L"classCacheExistsOnCmr").as_bool();

	// sensor type configs
	json::array configArray = json.at(L"platformSensorTypeConfigs").as_array();
	for (auto it = configArray.begin(); it != configArray.end(); it++) {
		auto platformSensorConfig = std::make_shared<PlatformSensorTypeConfig>();
		platformSensorConfig->fromJson(it->as_object());
		platformSensorTypeConfigs.push_back(platformSensorConfig);
	}

	configArray = json.at(L"methodSensorTypeConfigs").as_array();
	logger.debug("configArray: %ls", json.at(L"methodSensorTypeConfigs").serialize().c_str());
	for (auto it = configArray.begin(); it != configArray.end(); it++) {
		auto methodSensorConfig = std::make_shared<MethodSensorTypeConfig>();
		methodSensorConfig->fromJson(it->as_object());
		methodSensorTypeConfigs.push_back(methodSensorConfig);
	}

	exceptionSensorTypeConfig = std::make_shared<ExceptionSensorTypeConfig>();
	exceptionSensorTypeConfig->fromJson(json.at(L"exceptionSensorTypeConfig").as_object());

	configArray = json.at(L"methodSensorTypeConfigs").as_array();
	for (auto it = configArray.begin(); it != configArray.end(); it++) {
		auto methodSensorConfig = std::make_shared<MethodSensorTypeConfig>();
		methodSensorConfig->fromJson(it->as_object());
		specialMethodSensorTypeConfigs.push_back(methodSensorConfig);
	}

	// strategy configs
	bufferStrategyConfig = std::make_shared<StrategyConfig>();
	bufferStrategyConfig->fromJson(json.at(L"bufferStrategyConfig").as_object());

	sendingStrategyConfig = std::make_shared<StrategyConfig>();
	sendingStrategyConfig->fromJson(json.at(L"sendingStrategyConfig").as_object());

	// exclusions
	json::array excludesArray = json.at(L"excludeClassesPatterns").as_array();
	for (auto it = excludesArray.begin(); it != excludesArray.end(); it++) {
		std::wstring pattern = it->as_object().at(L"pattern").as_string();
		excludeClassesPatterns.push_back(pattern);
	}

	logger.debug("START");

	logger.debug("%ls", json.at(L"initialInstrumentationResults").serialize().c_str());

	// TODO: Breaks sometimes after this point

	// initial instr results
	json::object instrResultsMap = json.at(L"initialInstrumentationResults").as_object();
	logger.debug("1");
	for (auto it = instrResultsMap.begin(); it != instrResultsMap.end(); it++) {
		logger.debug("2");
		auto instrumentationDef = std::make_shared<InstrumentationDefinition>();
		instrumentationDef->fromJson(it->second.as_object());
		logger.debug("3");
		logger.debug("key: %ls", it->first.c_str());

		std::vector<std::wstring> keyList;
		std::wstring keyListStr = it->first.substr(1, it->first.size() - 2); // Params of substr are: begin index, number of chars (not end index)
		std::wistringstream keyListStream(keyListStr);
		std::wstring elem;

		while (std::getline(keyListStream, elem, L',')) {
			keyList.push_back(elem);
		}

		initialInstrumentationDefinitions.emplace(keyList, instrumentationDef);
		logger.debug("7");
	}

	logger.debug("END");

	// configuration info
	configurationInfo = json.at(L"configurationInfo").as_string();
}
