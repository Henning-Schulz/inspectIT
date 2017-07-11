#pragma once
#include "JsonReceivable.h"

#include <map>

class StrategyConfig :
	public JsonReceivable
{
private:
	std::wstring className;
	std::map<std::wstring, std::wstring> settings;

public:
	StrategyConfig();
	~StrategyConfig();

	std::wstring getClassName();
	std::map<std::wstring, std::wstring> getSettings();

	void fromJson(json::object json);
};