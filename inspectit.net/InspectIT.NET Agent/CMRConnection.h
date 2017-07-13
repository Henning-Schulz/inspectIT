#pragma once

#include "basehdr.h"
#include "StrategyConfig.h"
#include "AgentConfig.h"
#include "InstrumentationDefinition.h"
#include "Type.h"
#include "MethodSensorMap.h"
#include "SensorData.h"

#include <memory>

#include <vector>
#include <map>

namespace inspectit {
	namespace connection {

		class CMRConnection
		{
		public:
			CMRConnection() {}
			virtual ~CMRConnection() {}

			virtual void sendKeepAlive(JAVA_LONG platformId, bool waitForResponse) = 0;

			virtual std::shared_ptr<inspectit::config::AgentConfig> registerPlatform(LPWSTR agentName, LPWSTR version) = 0;
			virtual void unregisterPlatform(JAVA_LONG platformId) = 0;

			virtual std::shared_ptr<inspectit::config::InstrumentationDefinition> analyze(JAVA_LONG platformIdent, std::shared_ptr<inspectit::types::Type> type) = 0;
			virtual void instrumentationApplied(JAVA_LONG platformId, inspectit::data::MethodSensorMap methodToSensorMap, bool waitForResponse) = 0;

			// TODO: virtual void fetchAgentMessages(JAVA_LONG platformId) = 0;

			virtual void sendDataObjects(std::vector<std::shared_ptr<inspectit::data::SensorData>> dataObjects, bool waitForResponse) = 0;
		};

		bool setupCMRConnection();

		void shutdownCMRConnection();

	}
}

extern inspectit::connection::CMRConnection *cmrConnection;