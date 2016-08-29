#include "TimerSensorConfig.h"

const LPWSTR TimerSensorConfig::CLASS_NAME = L"rocks.inspectit.agent.java.sensor.method.timer.TimerSensor";
const LPWSTR TimerSensorConfig::SENSOR_NAME = L"Timer Sensor";


TimerSensorConfig::TimerSensorConfig()
{
}


TimerSensorConfig::~TimerSensorConfig()
{
}

LPWSTR TimerSensorConfig::getClassName()
{
	return CLASS_NAME;
}

LPWSTR TimerSensorConfig::getName()
{
	return SENSOR_NAME;
}

MethodSensorPriority TimerSensorConfig::getPriority()
{
	return MAX;
}
