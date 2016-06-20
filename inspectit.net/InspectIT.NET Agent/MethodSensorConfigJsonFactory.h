#pragma once

#include "MethodSensorConfig.h"
#include "Logger.h"

#include <memory>
#include <cpprest\json.h>

using namespace web;

class MethodSensorConfigJsonFactory
{
private:
	Logger logger = loggerFactory.createLogger("MethodSensorConfigJsonFactory");

public:
	MethodSensorConfigJsonFactory();
	~MethodSensorConfigJsonFactory();

	std::shared_ptr<MethodSensorConfig> createFromJson(json::object jsonObject);
};

