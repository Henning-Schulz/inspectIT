#pragma once
#include "MethodSensorConfig.h"
class TimerSensorConfig :
	public MethodSensorConfig
{
public:
	static const LPWSTR CLASS_NAME;
	static const LPWSTR SENSOR_NAME;

	TimerSensorConfig();
	~TimerSensorConfig();

	LPWSTR getClassName();
	LPWSTR getName();
	MethodSensorPriority getPriority();
};

