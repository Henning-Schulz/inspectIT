#pragma once

#include "basehdr.h"
#include "SensorData.h"

#include <cpprest/json.h>
#include <chrono>

#define TYPE_KEY L"objectType"

namespace inspectit {
	namespace data {

		class MethodSensorData : public SensorData
		{
		private:
			typedef SensorData super;

			JAVA_LONG methodId;

		public:
			MethodSensorData(JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, JAVA_LONG timestamp = -1)
				: SensorData(platformId, methodSensorId, timestamp)
			{
				this->methodId = methodId;
			}

			virtual ~MethodSensorData() {}

			JAVA_LONG getMethodId() { return methodId; }
			JAVA_LONG getIdentifyingId() { return getMethodId(); }

			virtual web::json::value toJson()
			{
				web::json::value json = super::toJson();
				json[L"methodIdent"] = web::json::value::number(getMethodId());
				return json;
			}

		};

	}
}