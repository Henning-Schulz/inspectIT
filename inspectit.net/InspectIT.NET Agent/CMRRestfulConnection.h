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
	std::wstring baseUrl;
	std::wstring connectionTestPath;
	std::wstring keepAlivePath;
	std::wstring agentPath;
	std::wstring storagePath;

	Logger logger = loggerFactory.createLogger("CMRRestfulConnection");

public:
	CMRRestfulConnection();
	~CMRRestfulConnection();

	bool testConnection();

	void sendKeepAlive(JAVA_LONG platformId, bool waitForResponse);

	std::shared_ptr<AgentConfig> registerPlatform(LPWSTR agentName, LPWSTR version);
	void unregisterPlatform(JAVA_LONG platformId);

	std::shared_ptr<InstrumentationDefinition> analyze(JAVA_LONG platformIdent, std::shared_ptr<Type> type);
	void instrumentationApplied(JAVA_LONG platformId, MethodSensorMap methodToSensorMap, bool waitForResponse);

	void sendDataObjects(std::vector<std::shared_ptr<SensorData>> dataObjects, bool waitForResponse);
};

#define MIME_JSON L"application/json"

