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

std::shared_ptr<DataSendingService> MethodSensor::getDataSendingService()
{
	return dataSendingService;
}

void MethodSensor::initialize(std::shared_ptr<MethodSensorTypeConfig> config, JAVA_LONG platformId, ICorProfilerInfo * profilerInfo, std::shared_ptr<DataSendingService> dataSendingService)
{
	this->sensorTypeId = config->getId();
	this->platformId = platformId;
	this->name = config->getName();
	this->dataSendingService = dataSendingService;

	init(profilerInfo);
}
