#include "AbstractSensorTypeConfig.h"



AbstractSensorTypeConfig::AbstractSensorTypeConfig()
{
}


AbstractSensorTypeConfig::~AbstractSensorTypeConfig()
{
}

JAVA_LONG AbstractSensorTypeConfig::getId()
{
	return id;
}

std::wstring AbstractSensorTypeConfig::getClassName()
{
	return className;
}

std::map<std::wstring, std::wstring> AbstractSensorTypeConfig::getParameters()
{
	return std::map<std::wstring, std::wstring>();
}

void AbstractSensorTypeConfig::fromJson(json::object json)
{
	logger.debug("1");
	id = json.at(L"id").as_number().to_int64();
	logger.debug("2");
	className = json.at(L"className").as_string();

	logger.debug("3");
	json::object paramsMap = json.at(L"parameters").as_object();
	logger.debug("4");
	for (auto it = paramsMap.begin(); it != paramsMap.end(); it++) {
		logger.debug("5");
		logger.debug("Key is %ls", it->first.c_str());
		logger.debug("Value is %ls", it->second.serialize().c_str());
		parameters.emplace(it->first, it->second.serialize());
	}

	logger.debug("6");
}
