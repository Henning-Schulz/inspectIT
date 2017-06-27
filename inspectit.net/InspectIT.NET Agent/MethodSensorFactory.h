#pragma once

#include "MethodSensor.h"
#include "MethodSensorTypeConfig.h"
#include <shared_mutex>

class MethodSensorFactory
{
private:
	std::map<std::wstring, std::shared_ptr<MethodSensor>> knownSensors;

	void setupKnownSensors();
	void addKnownSensor(std::shared_ptr<MethodSensor> sensor);

public:
	MethodSensorFactory();
	~MethodSensorFactory();

	std::shared_ptr<MethodSensor> createMethodSensor(std::shared_ptr<MethodSensorTypeConfig> sensorTypeConfig, JAVA_LONG platformId, ICorProfilerInfo3 *profilerInfo);
};