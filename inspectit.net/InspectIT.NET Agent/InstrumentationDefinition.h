#pragma once
#include "JsonReceivable.h"
#include "MethodInstrumentationConfig.h"

#include <map>

namespace inspectit {
	namespace config {

		class InstrumentationDefinition :
			public inspectit::json::JsonReceivable
		{
		private:
			std::wstring className;
			std::vector<std::shared_ptr<MethodInstrumentationConfig>> methodInstrumentationConfigs;

		public:
			InstrumentationDefinition();
			~InstrumentationDefinition();

			std::wstring getClassName();
			std::vector<std::shared_ptr<MethodInstrumentationConfig>> getMethodInstrumentationConfigs();

			void fromJson(web::json::object json);
		};

	}
}