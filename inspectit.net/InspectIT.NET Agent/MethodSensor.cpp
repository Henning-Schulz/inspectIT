#include "MethodSensor.h"

JAVA_LONG MethodSensor::getSensorTypeId()
{
	return sensorTypeId;
}

JAVA_LONG MethodSensor::getPlatformId()
{
	return platformId;
}

std::wstring MethodSensor::getName()
{
	return name;
}

void MethodSensor::initialize(std::shared_ptr<MethodSensorTypeConfig> config, JAVA_LONG platformId, ICorProfilerInfo * profilerInfo)
{
	this->sensorTypeId = config->getId();
	this->platformId = platformId;
	this->name = config->getName();

	init(profilerInfo);
}
