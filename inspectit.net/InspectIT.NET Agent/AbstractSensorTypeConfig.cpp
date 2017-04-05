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

std::map<std::wstring, json::object> AbstractSensorTypeConfig::getParameters()
{
	return std::map<std::wstring, json::object>();
}

void AbstractSensorTypeConfig::fromJson(json::object json)
{
	id = json.at(L"id").as_number().to_int64();
	className = json.at(L"className").as_string();

	json::object paramsMap = json.at(L"parameters").as_object();
	for (auto it = paramsMap.begin(); it != paramsMap.end(); it++) {
		parameters.emplace(it->first, it->second.as_object());
	}
}
