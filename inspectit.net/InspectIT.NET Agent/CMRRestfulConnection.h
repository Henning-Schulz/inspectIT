#pragma once

#include "CMRConnection.h"
#include "Logger.h"

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <sstream>
#include <cpprest/json.h>

class CMRRestfulConnection : public CMRConnection
{
private:
	LPWSTR baseUrl;
	LPWSTR registrationPath;
	LPWSTR connectionTestPath;
	LPWSTR storagePath;

	Logger logger = loggerFactory.createLogger("CMRRestfulConnection");

public:
	CMRRestfulConnection();
	~CMRRestfulConnection();

	bool testConnection();

	JAVA_LONG registerPlatform(std::vector<std::string> definedIps, LPWSTR agentName, LPWSTR version);
	void unregisterPlatform(std::vector<std::string> definedIps, LPWSTR agentName);

	JAVA_LONG registerMethod(JAVA_LONG platformId, LPWSTR className, LPWSTR methodName, LPWSTR returnType, std::vector<LPWSTR> parameterTypes, JAVA_INT modifiers);

	JAVA_LONG registerMethodSensorType(JAVA_LONG platformId, LPWSTR agentClassName);

	void addSensorTypeToMethod(JAVA_LONG sensorTypeId, JAVA_LONG methodId);

	void sendDataObjects(std::vector<std::shared_ptr<MethodSensorData>> dataObjects, bool waitForResponse);
};

#define MIME_JSON L"application/json"

