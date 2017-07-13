#include "AbstractSensorTypeConfig.h"

using namespace inspectit::config;

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

void AbstractSensorTypeConfig::fromJson(web::json::object json)
{
	id = json.at(L"id").as_number().to_int64();
	className = json.at(L"className").as_string();

	web::json::object paramsMap = json.at(L"parameters").as_object();
	for (auto it = paramsMap.begin(); it != paramsMap.end(); it++) {
		logger.debug("Key is %ls", it->first.c_str());
		logger.debug("Value is %ls", it->second.serialize().c_str());
		parameters.emplace(it->first, it->second.serialize());
	}
}
