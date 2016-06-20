#pragma once

#include "StrategyConfig.h"

#include <memory>
#include <cpprest/json.h>

using namespace web;

class StrategyConfigJsonFactory
{
public:
	StrategyConfigJsonFactory();
	~StrategyConfigJsonFactory();

	std::shared_ptr<StrategyConfig> createFromJson(json::object jsonObject);
};

