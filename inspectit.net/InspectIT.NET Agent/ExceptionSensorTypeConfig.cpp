#include "ExceptionSensorTypeConfig.h"



ExceptionSensorTypeConfig::ExceptionSensorTypeConfig()
{
}


ExceptionSensorTypeConfig::~ExceptionSensorTypeConfig()
{
}

bool ExceptionSensorTypeConfig::isEnhanced()
{
	return enhanced;
}

void ExceptionSensorTypeConfig::fromJson(json::object json)
{
	super::fromJson(json);

	enhanced = json.at(L"enhanced").as_bool();
}
