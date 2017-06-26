#include "StrategyConfig.h"



StrategyConfig::StrategyConfig()
{
}


StrategyConfig::~StrategyConfig()
{
}

std::wstring StrategyConfig::getClassName()
{
	return className;
}

std::map<std::wstring, std::wstring> StrategyConfig::getSettings()
{
	return settings;
}

void StrategyConfig::fromJson(json::object json)
{
	className = json.at(L"clazzName").as_string();

	json::object settingsMap = json.at(L"settings").as_object();
	for (auto it = settingsMap.begin(); it != settingsMap.end(); it++) {
		settings.emplace(it->first, it->second.as_string());
	}
}