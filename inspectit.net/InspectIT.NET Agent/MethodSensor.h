#pragma once

#include "MethodHook.h"
#include "MethodSensorConfig.h"
#include "ThreadHook.h"

#include <memory>

class MethodSensor
{

private:
	JAVA_LONG sensorTypeId;
	JAVA_LONG platformId;

public:
	MethodSensor() {}
	virtual ~MethodSensor() {}

	virtual void init(std::shared_ptr<MethodSensorConfig> config, JAVA_LONG sensorTypeId, JAVA_LONG platformId, ICorProfilerInfo *profilerInfo) = 0;
	virtual void notifyShutdown() = 0;

	virtual bool hasHook() = 0;
	virtual std::shared_ptr<MethodHook> getHook() = 0;

	virtual bool hasThreadHook() = 0;
	virtual std::shared_ptr<ThreadHook> getThreadHook() = 0;

	virtual DWORD getSpecialMonitorFlags() = 0;

	JAVA_LONG getSensorTypeId() { return sensorTypeId; }

	JAVA_LONG getPlatformId() { return platformId; }
};