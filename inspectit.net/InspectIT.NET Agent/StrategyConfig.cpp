#include "StrategyConfig.h"

using namespace inspectit::config;

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

std::wstring inspectit::config::StrategyConfig::getSetting(std::wstring key)
{
	auto it = settings.find(key);
	if (it != settings.end()) {
		return it->second;
	}
	else {
		return L"";
	}
}

int inspectit::config::StrategyConfig::getIntegerSetting(std::wstring key)
{
	std::wstring strVal = getSetting(key);
	return _wtoi(strVal.c_str());
}

void StrategyConfig::fromJson(web::json::object json)
{
	className = json.at(L"className").as_string();

	web::json::object settingsMap = json.at(L"settings").as_object();
	for (auto it = settingsMap.begin(); it != settingsMap.end(); it++) {
		settings.emplace(it->first, it->second.as_string());
	}
}
