#include "MethodHook.h"

std::shared_ptr<DataSendingService> MethodHook::getDataSendingService()
{
	return dataSendingService;
}

void MethodHook::initialize(ICorProfilerInfo * profilerInfo, std::shared_ptr<DataSendingService> dataSendingService)
{
	this->dataSendingService = dataSendingService;
	init(profilerInfo);
}


MethodSensorPriority MethodHook::getPriority()
{
	return priority;
}

void MethodHook::setPriority(MethodSensorPriority priority)
{
	this->priority = priority;
}

void MethodHook::setSensorTypeId(JAVA_LONG sensorTypeId)
{
	this->sensorTypeId = sensorTypeId;
}

JAVA_LONG MethodHook::getSensorTypeId()
{
	return sensorTypeId;
}

void MethodHook::setPlatformId(JAVA_LONG platformId)
{
	this->platformId = platformId;
}

JAVA_LONG MethodHook::getPlatformId()
{
	return platformId;
}