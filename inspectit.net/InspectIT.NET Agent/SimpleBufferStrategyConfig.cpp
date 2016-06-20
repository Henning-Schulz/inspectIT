#include "SimpleBufferStrategyConfig.h"

const LPWSTR SimpleBufferStrategyConfig::CLASS_NAME = L"rocks.inspectit.agent.java.buffer.impl.SimpleBufferStrategy";

SimpleBufferStrategyConfig::SimpleBufferStrategyConfig()
{
}


SimpleBufferStrategyConfig::~SimpleBufferStrategyConfig()
{
}

LPWSTR SimpleBufferStrategyConfig::getClassName()
{
	return CLASS_NAME;
}