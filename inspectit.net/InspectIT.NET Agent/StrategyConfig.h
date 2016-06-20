#pragma once

#include "basehdr.h"

#include <map>

class StrategyConfig
{
public:
	StrategyConfig() {}
	virtual ~StrategyConfig() {}

	virtual LPWSTR getClassName() = 0;
};