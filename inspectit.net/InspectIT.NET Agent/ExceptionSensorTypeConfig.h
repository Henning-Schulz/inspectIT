#pragma once
#include "MethodSensorTypeConfig.h"

class ExceptionSensorTypeConfig : MethodSensorTypeConfig
{
private:
	typedef MethodSensorTypeConfig super;

	bool enhanced;

public:
	ExceptionSensorTypeConfig();
	~ExceptionSensorTypeConfig();

	bool isEnhanced();

	void fromJson(json::object json);
};

