#pragma once

#include "basehdr.h"

/*
 * Interface for method sensors.
 */
class MethodSensor {

private:
	JAVA_LONG sensorTypeId;
	JAVA_LONG platformId;

public:
	MethodSensor() {}
	virtual ~MethodSensor() {}

	virtual void init(ICorProfilerInfo *profilerInfo) = 0;
	virtual void notifyShutdown() = 0;

	virtual void beforeBody(METHOD_ID methodID) = 0;
	virtual void afterBody(METHOD_ID methodID) = 0;

	void setSensorTypeId(JAVA_LONG sensorTypeId) { this->sensorTypeId = sensorTypeId; }
	JAVA_LONG getSensorTypeId() { return sensorTypeId; }

	void setPlatformId(JAVA_LONG platformId) { this->platformId = platformId; }
	JAVA_LONG getPlatformId() { return platformId; }

};
