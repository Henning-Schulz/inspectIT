#pragma once

#include "MethodHook.h"
#include "Logger.h"
#include "SensorDataWrapper.h"

namespace inspectit {
	namespace sensor {
		namespace timer {

			class TimerHook : public MethodHook
			{
			private:
				inspectit::logger::Logger logger = loggerFactory.createLogger("TimerHook");

				ICorProfilerInfo *profilerInfo;

			protected:
				void init(ICorProfilerInfo *profilerInfo);

			public:
				TimerHook();
				~TimerHook();
				void notifyShutdown();

				void beforeBody(METHOD_ID methodID);
				void afterBody(METHOD_ID methodID);
			};

		}
	}
}