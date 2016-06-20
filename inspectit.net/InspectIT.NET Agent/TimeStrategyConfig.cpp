#include "TimeStrategyConfig.h"



const LPWSTR TimeStrategyConfig::CLASS_NAME = L"rocks.inspectit.agent.java.sending.impl.TimeStrategy";

TimeStrategyConfig::TimeStrategyConfig()
{
}


TimeStrategyConfig::~TimeStrategyConfig()
{
}

LPWSTR TimeStrategyConfig::getClassName()
{
	return CLASS_NAME;
}

JAVA_LONG TimeStrategyConfig::getTime()
{
	return time;
}

void TimeStrategyConfig::setTime(JAVA_LONG time)
{
	this->time = time;
}