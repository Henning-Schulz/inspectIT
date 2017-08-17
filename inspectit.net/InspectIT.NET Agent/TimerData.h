#pragma once

#include "MethodSensorData.h"

namespace inspectit {
	namespace sensor {
		namespace timer {

			class TimerData :
				public inspectit::data::MethodSensorData
			{
			private:
				typedef inspectit::data::MethodSensorData super;

				JAVA_INT threadId;

				JAVA_LONG startTime;
				JAVA_LONG endTime;

			protected:
				std::wstring typeName();

			public:
				TimerData(JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, JAVA_INT threadId, JAVA_LONG timestamp);
				~TimerData();

				void setStartTime(JAVA_LONG startTime);
				void setEndTime(JAVA_LONG endTime);

				web::json::value toJson();
			};

		}
	}
}