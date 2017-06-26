#pragma once

#include "basehdr.h"
#include "SensorInstrumentationPoint.h"
#include "ClassType.h"
#include "InstrumentationDefinition.h"
#include "MethodSensor.h"

#include <map>
#include <vector>
#include <string>

class InstrumentationManager
{
private:
	JAVA_LONG platformId;
	ICorProfilerInfo3 *profilerInfo;

	std::vector<std::wstring> excludeClassesPatterns;
	std::map<FunctionID, std::shared_ptr<SensorInstrumentationPoint>> sensorInstrumentationPoints;

	std::map<JAVA_LONG, std::shared_ptr<MethodSensorTypeConfig>> sensorTypeConfigs;
	std::map<JAVA_LONG, std::shared_ptr<MethodSensor>> methodSensors;

public:
	InstrumentationManager(JAVA_LONG platformId, ICorProfilerInfo3 *profilerInfo);
	~InstrumentationManager();

	void setExcludeClassesPatterns(std::vector<std::wstring> excludeClassesPatterns);
	bool instrumentationDefinitionMatches(std::shared_ptr<MethodType> methodType, std::shared_ptr<MethodInstrumentationConfig> instrConfig);

	void registerInstrumentationDefinition(std::shared_ptr<ClassType> classType, std::shared_ptr<InstrumentationDefinition> instrDef);
	std::shared_ptr<SensorInstrumentationPoint> getSensorInstrumentationPoint(FunctionID functionId);

	std::shared_ptr<MethodSensor> getMethodSensorForId(JAVA_LONG sensorId);
};

