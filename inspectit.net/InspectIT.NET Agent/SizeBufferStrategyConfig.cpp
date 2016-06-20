#include "SizeBufferStrategyConfig.h"

const LPWSTR SizeBufferStrategyConfig::CLASS_NAME = L"rocks.inspectit.agent.java.buffer.impl.SizeBufferStrategy";

SizeBufferStrategyConfig::SizeBufferStrategyConfig()
{
}


SizeBufferStrategyConfig::~SizeBufferStrategyConfig()
{
}

LPWSTR SizeBufferStrategyConfig::getClassName()
{
	return CLASS_NAME;
}

JAVA_INT SizeBufferStrategyConfig::getSize()
{
	return size;
}

void SizeBufferStrategyConfig::setSize(JAVA_INT size)
{
	this->size = size;
}