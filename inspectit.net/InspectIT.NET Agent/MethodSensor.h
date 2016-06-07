#pragma once

#include "basehdr.h"

/*
 * Interface for method sensors.
 */
class MethodSensor {

private:
	JAVA_LONG sensorTypeId;

public:
	MethodSensor() {}
	virtual ~MethodSensor() {}

	virtual void beforeBody(METHOD_ID methodID) = 0;
	virtual void afterBody(METHOD_ID methodID) = 0;

	void setSensorTypeId(JAVA_LONG sensorTypeId) { MethodSensor::sensorTypeId = sensorTypeId; }
	JAVA_LONG getSensorTypeId() { return sensorTypeId; }

};
