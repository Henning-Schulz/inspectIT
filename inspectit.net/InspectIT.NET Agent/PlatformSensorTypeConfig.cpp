#include "PlatformSensorTypeConfig.h"

using namespace inspectit::config;

PlatformSensorTypeConfig::PlatformSensorTypeConfig()
{
}


PlatformSensorTypeConfig::~PlatformSensorTypeConfig()
{
}

void PlatformSensorTypeConfig::fromJson(web::json::object json)
{
	super::fromJson(json);
}
