#pragma once
#include "AbstractSensorTypeConfig.h"

namespace inspectit {
	namespace config {

		class PlatformSensorTypeConfig :
			public AbstractSensorTypeConfig
		{
		private:
			typedef AbstractSensorTypeConfig super;

		public:
			PlatformSensorTypeConfig();
			~PlatformSensorTypeConfig();

			void fromJson(web::json::object json);
		};
	}
}
