#pragma once
#include "AbstractSensorTypeConfig.h"
#include "Logger.h"

namespace inspectit {
	namespace config {

		enum MethodSensorPriority { INVOC = 1, MIN, LOW, NORMAL, HIGH, MAX };

		class MethodSensorTypeConfig :
			public AbstractSensorTypeConfig
		{
		private:
			typedef AbstractSensorTypeConfig super;

			std::wstring name;
			MethodSensorPriority priority = NORMAL;

			MethodSensorPriority prioFromString(std::wstring sPrio);

			inspectit::logger::Logger logger = loggerFactory.createLogger("MethodSensorTypeConfig");

		public:
			MethodSensorTypeConfig();
			~MethodSensorTypeConfig();

			std::wstring getName();
			MethodSensorPriority getPriority();

			void fromJson(web::json::object json);
		};

	}
}