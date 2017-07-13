#include "ExceptionSensorTypeConfig.h"

using namespace inspectit::config;

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

void ExceptionSensorTypeConfig::fromJson(web::json::object json)
{
	super::fromJson(json);

	enhanced = json.at(L"enhanced").as_bool();
}
