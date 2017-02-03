#include "TimerSensor.h"



TimerSensor::TimerSensor()
{
	this->hook = std::make_shared<TimerHook>();
}


TimerSensor::~TimerSensor()
{
}

void TimerSensor::init(std::shared_ptr<MethodSensorConfig> config, JAVA_LONG sensorTypeId, JAVA_LONG platformId, ICorProfilerInfo *profilerInfo)
{
	setSensorTypeId(sensorTypeId);
	setPlatformId(platformId);
	hook->setPlatformId(platformId);
	hook->setSensorTypeId(sensorTypeId);
	hook->init(profilerInfo);
}

void TimerSensor::notifyStartup()
{
}

void TimerSensor::notifyShutdown()
{
}

bool TimerSensor::hasHook()
{
	return true;
}

std::shared_ptr<MethodHook> TimerSensor::getHook()
{
	return hook;
}

bool TimerSensor::hasThreadHook()
{
	return false;
}

std::shared_ptr<ThreadHook> TimerSensor::getThreadHook()
{
	return std::shared_ptr<ThreadHook>();
}

DWORD TimerSensor::getSpecialMonitorFlags()
{
	return 0;
}