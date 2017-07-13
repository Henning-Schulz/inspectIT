#pragma once
#include "JsonReceivable.h"

#include <map>

namespace inspectit {
	namespace config {

		class StrategyConfig :
			public inspectit::json::JsonReceivable
		{
		private:
			std::wstring className;
			std::map<std::wstring, std::wstring> settings;

		public:
			StrategyConfig();
			~StrategyConfig();

			std::wstring getClassName();
			std::map<std::wstring, std::wstring> getSettings();

			void fromJson(web::json::object json);
		};
	}
}