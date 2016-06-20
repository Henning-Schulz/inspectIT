#pragma once

#include "StrategyConfig.h"

class SimpleBufferStrategyConfig : public StrategyConfig
{
public:
	static const LPWSTR CLASS_NAME;

	SimpleBufferStrategyConfig();
	~SimpleBufferStrategyConfig();

	LPWSTR getClassName();
};

