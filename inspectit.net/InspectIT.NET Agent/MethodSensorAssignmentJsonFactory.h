#pragma once

#include "MethodSensorAssignment.h"
#include "Logger.h"

#include <memory>
#include <cpprest\json.h>

using namespace web;

class MethodSensorAssignmentJsonFactory
{
private:
	Logger logger = loggerFactory.createLogger("MethodSensorAssignmentJsonFactory");

public:
	MethodSensorAssignmentJsonFactory();
	~MethodSensorAssignmentJsonFactory();

	std::shared_ptr<MethodSensorAssignment> createFromJson(json::object jsonObject);
};

