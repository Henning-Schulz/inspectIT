#pragma once

#include "basehdr.h"
#include "MethodSensorData.h"
#include "StrategyConfig.h"
#include "MethodSensorConfig.h"
#include "MethodSensorAssignment.h"

#include <memory>

#include <vector>
#include <map>

class CMRConnection
{
public:
	CMRConnection() {}
	virtual ~CMRConnection() {}

	virtual JAVA_LONG registerPlatform(std::vector<std::string> definedIps, LPWSTR agentName, LPWSTR version) = 0;
	virtual void unregisterPlatform(std::vector<std::string> definedIps, LPWSTR agentName) = 0;

	virtual JAVA_LONG registerMethod(JAVA_LONG platformId, LPWSTR className, LPWSTR methodName, LPWSTR returnType, std::vector<LPWSTR> parameterTypes, JAVA_INT modifiers) = 0;

	virtual JAVA_LONG registerMethodSensorType(JAVA_LONG platformId, LPWSTR agentClassName) = 0;
	virtual void addSensorTypeToMethod(JAVA_LONG sensorTypeId, JAVA_LONG methodId) = 0;

	virtual std::shared_ptr<StrategyConfig> getSendingStrategyConfig(JAVA_LONG platformId) = 0;
	virtual std::shared_ptr<StrategyConfig> getBufferStrategyConfig(JAVA_LONG platformId) = 0;
	virtual std::vector<std::shared_ptr<MethodSensorConfig>> getMethodSensorConfigs(JAVA_LONG platformId) = 0;
	virtual std::vector<std::shared_ptr<MethodSensorAssignment>> getMethodSensorAssignments(JAVA_LONG platformId) = 0;
	virtual std::vector<std::wstring> getExcludedClasses(JAVA_LONG platformId) = 0;

	virtual void sendDataObjects(std::vector<std::shared_ptr<MethodSensorData>> dataObjects, bool waitForResponse) = 0;
	virtual void sendKeepAlive(JAVA_LONG platformId, bool waitForResponse) = 0;
};

extern CMRConnection *cmrConnection;

bool setupCMRConnection();

void shutdownCMRConnection();