#pragma once

#include "basehdr.h"
#include "SensorInstrumentationPoint.h"
#include "ClassType.h"
#include "InstrumentationDefinition.h"
#include "MethodSensor.h"
#include "MethodSensorFactory.h"
#include "DataSendingService.h"
#include "Logger.h"

#include <map>
#include <vector>
#include <string>

class InstrumentationManager
{
private:
	JAVA_LONG platformId;
	ICorProfilerInfo3 *profilerInfo;

	std::vector<std::wstring> excludeClassesPatterns;
	std::map<mdMethodDef, std::shared_ptr<SensorInstrumentationPoint>> sensorInstrumentationPoints;

	std::map<JAVA_LONG, std::shared_ptr<MethodSensorTypeConfig>> sensorTypeConfigs;
	std::map<JAVA_LONG, std::shared_ptr<MethodSensor>> methodSensors;

	MethodSensorFactory methodSensorFactory;

	std::shared_ptr<DataSendingService> dataSendingService;

	Logger logger = loggerFactory.createLogger("InstrumentationManager");

public:
	InstrumentationManager(JAVA_LONG platformId, ICorProfilerInfo3 *profilerInfo, std::shared_ptr<DataSendingService> dataSendingService);
	~InstrumentationManager();

	void addSensorTypeConfigs(std::vector<std::shared_ptr<MethodSensorTypeConfig>> configs);

	void setExcludeClassesPatterns(std::vector<std::wstring> excludeClassesPatterns);
	bool instrumentationDefinitionMatches(std::shared_ptr<MethodType> methodType, std::shared_ptr<MethodInstrumentationConfig> instrConfig);

	void registerInstrumentationDefinition(std::shared_ptr<ClassType> classType, std::shared_ptr<InstrumentationDefinition> instrDef);
	std::shared_ptr<SensorInstrumentationPoint> getSensorInstrumentationPoint(mdMethodDef methodDef);

	std::shared_ptr<MethodSensor> getMethodSensorForId(JAVA_LONG sensorId);
};

