#pragma once

#include "basehdr.h"
#include "JsonReceivable.h"

#include <map>

namespace inspectit {
	namespace config {

		class SensorInstrumentationPoint :
			public inspectit::json::JsonReceivable
		{
		private:
			JAVA_LONG id;
			std::vector<JAVA_LONG> sensorIds;
			std::vector<byte> sensorPriorities;
			bool constructor;
			bool startsInvocation;
			std::map<std::wstring, web::json::object> settings;

			// TODO: propertyAccessorList

		public:
			SensorInstrumentationPoint();
			~SensorInstrumentationPoint();

			JAVA_LONG getId();
			std::vector<JAVA_LONG> getSensorIds();
			std::vector<byte> getSensorPriorities();
			bool isConstructor();
			bool doesStartInvocation();
			std::map<std::wstring, web::json::object> getSettings();

			void fromJson(web::json::object json);
		};

	}
}