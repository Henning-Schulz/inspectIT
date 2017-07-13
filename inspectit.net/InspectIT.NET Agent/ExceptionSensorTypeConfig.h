#pragma once
#include "MethodSensorTypeConfig.h"

namespace inspectit {
	namespace config {

		class ExceptionSensorTypeConfig : MethodSensorTypeConfig
		{
		private:
			typedef MethodSensorTypeConfig super;

			bool enhanced;

		public:
			ExceptionSensorTypeConfig();
			~ExceptionSensorTypeConfig();

			bool isEnhanced();

			void fromJson(web::json::object json);
		};

	}
}