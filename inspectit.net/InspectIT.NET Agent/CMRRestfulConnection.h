#pragma once

#include "CMRConnection.h"
#include "Logger.h"
#include "StrategyConfigJsonFactory.h"
#include "MethodSensorConfigJsonFactory.h"
#include "MethodSensorAssignmentJsonFactory.h"

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <sstream>
#include <cpprest/json.h>

class CMRRestfulConnection : public CMRConnection
{
private:
	std::wstring baseUrl;
	std::wstring registrationPath;
	std::wstring connectionTestPath;
	std::wstring configPath;
	std::wstring storagePath;

	StrategyConfigJsonFactory strategyConfigFactory = StrategyConfigJsonFactory();
	MethodSensorConfigJsonFactory sensorConfigFactory = MethodSensorConfigJsonFactory();
	MethodSensorAssignmentJsonFactory sensorAssignmentFactory = MethodSensorAssignmentJsonFactory();

	Logger logger = loggerFactory.createLogger("CMRRestfulConnection");

	std::shared_ptr<StrategyConfig> getStrategyConfig(JAVA_LONG platformId, const wchar_t* path);

public:
	CMRRestfulConnection();
	~CMRRestfulConnection();

	bool testConnection();

	JAVA_LONG registerPlatform(std::vector<std::string> definedIps, LPWSTR agentName, LPWSTR version);
	void unregisterPlatform(std::vector<std::string> definedIps, LPWSTR agentName);

	JAVA_LONG registerMethod(JAVA_LONG platformId, LPWSTR className, LPWSTR methodName, LPWSTR returnType, std::vector<LPWSTR> parameterTypes, JAVA_INT modifiers);

	JAVA_LONG registerMethodSensorType(JAVA_LONG platformId, LPWSTR agentClassName);

	void addSensorTypeToMethod(JAVA_LONG sensorTypeId, JAVA_LONG methodId);

	std::shared_ptr<StrategyConfig> getSendingStrategyConfig(JAVA_LONG platformId);
	std::shared_ptr<StrategyConfig> getBufferStrategyConfig(JAVA_LONG platformId);
	std::vector<std::shared_ptr<MethodSensorConfig>> getMethodSensorConfigs(JAVA_LONG platformId);
	std::vector<std::shared_ptr<MethodSensorAssignment>> getMethodSensorAssignments(JAVA_LONG platformId);

	void sendDataObjects(std::vector<std::shared_ptr<MethodSensorData>> dataObjects, bool waitForResponse);
	void sendKeepAlive(JAVA_LONG platformId, bool waitForResponse);
};

#define MIME_JSON L"application/json"

