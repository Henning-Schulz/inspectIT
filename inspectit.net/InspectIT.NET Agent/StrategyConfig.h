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

			std::wstring getSetting(std::wstring key);
			int getIntegerSetting(std::wstring key);

			void fromJson(web::json::object json);
		};
	}
}