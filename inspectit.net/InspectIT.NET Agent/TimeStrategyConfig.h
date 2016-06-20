#pragma once

#include "StrategyConfig.h"

class TimeStrategyConfig : public StrategyConfig
{
private:
	JAVA_LONG time = 5000;

public:
	const static LPWSTR CLASS_NAME;

	TimeStrategyConfig();
	~TimeStrategyConfig();

	LPWSTR getClassName();

	JAVA_LONG getTime();
	void setTime(JAVA_LONG time);
};