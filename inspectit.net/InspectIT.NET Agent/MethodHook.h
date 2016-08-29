#pragma once

#include "basehdr.h"

#include "MethodSensorConfig.h"

/*
 * Interface for method sensors.
 */
class MethodHook {

private:
	JAVA_LONG sensorTypeId;
	JAVA_LONG platformId;
	MethodSensorPriority priority = MIN;

public:
	MethodHook() {}
	virtual ~MethodHook() {}

	MethodSensorPriority getPriority() { return priority; }
	void setPriority(MethodSensorPriority priority) { this->priority = priority; }

	virtual void init(ICorProfilerInfo *profilerInfo) = 0;
	virtual void notifyShutdown() = 0;

	virtual void beforeBody(METHOD_ID methodID) = 0;
	virtual void afterBody(METHOD_ID methodID) = 0;

	void setSensorTypeId(JAVA_LONG sensorTypeId) { this->sensorTypeId = sensorTypeId; }
	JAVA_LONG getSensorTypeId() { return sensorTypeId; }

	void setPlatformId(JAVA_LONG platformId) { this->platformId = platformId; }
	JAVA_LONG getPlatformId() { return platformId; }
};
