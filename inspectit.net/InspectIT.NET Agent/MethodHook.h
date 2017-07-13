#pragma once

#include "basehdr.h"
#include "MethodSensorTypeConfig.h"
#include "DataSendingService.h"

namespace inspectit {
	namespace sensor {

		/*
		 * Interface for method sensors.
		 */
		class MethodHook {

		private:
			JAVA_LONG sensorTypeId;
			JAVA_LONG platformId;
			inspectit::config::MethodSensorPriority priority = inspectit::config::MIN;

			std::shared_ptr<inspectit::sending::DataSendingService> dataSendingService;

		protected:
			virtual void init(ICorProfilerInfo *profilerInfo) = 0;

			std::shared_ptr<inspectit::sending::DataSendingService> getDataSendingService();

		public:
			MethodHook() {}
			virtual ~MethodHook() {}

			inspectit::config::MethodSensorPriority getPriority();
			void setPriority(inspectit::config::MethodSensorPriority priority);

			void initialize(ICorProfilerInfo *profilerInfo, std::shared_ptr<inspectit::sending::DataSendingService> dataSendingService);
			virtual void notifyShutdown() = 0;

			virtual void beforeBody(METHOD_ID methodID) = 0;
			virtual void afterBody(METHOD_ID methodID) = 0;

			void setSensorTypeId(JAVA_LONG sensorTypeId);
			JAVA_LONG getSensorTypeId();

			void setPlatformId(JAVA_LONG platformId);
			JAVA_LONG getPlatformId();
		};

	}
}