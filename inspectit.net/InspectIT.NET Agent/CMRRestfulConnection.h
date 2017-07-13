#pragma once

#include "CMRConnection.h"
#include "Logger.h"

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <sstream>
#include <cpprest/json.h>

namespace inspectit {
	namespace connection {

		class CMRRestfulConnection : public CMRConnection
		{
		private:
			std::wstring baseUrl;
			std::wstring connectionTestPath;
			std::wstring keepAlivePath;
			std::wstring agentPath;
			std::wstring storagePath;

			inspectit::logger::Logger logger = loggerFactory.createLogger("CMRRestfulConnection");

		public:
			CMRRestfulConnection();
			~CMRRestfulConnection();

			bool testConnection();

			void sendKeepAlive(JAVA_LONG platformId, bool waitForResponse);

			std::shared_ptr<inspectit::config::AgentConfig> registerPlatform(LPWSTR agentName, LPWSTR version);
			void unregisterPlatform(JAVA_LONG platformId);

			std::shared_ptr<inspectit::config::InstrumentationDefinition> analyze(JAVA_LONG platformIdent, std::shared_ptr<inspectit::types::Type> type);
			void instrumentationApplied(JAVA_LONG platformId, inspectit::data::MethodSensorMap methodToSensorMap, bool waitForResponse);

			void sendDataObjects(std::vector<std::shared_ptr<inspectit::data::SensorData>> dataObjects, bool waitForResponse);
		};

	#define MIME_JSON L"application/json"

	}
}