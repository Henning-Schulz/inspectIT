#pragma once

#include "MethodSensor.h"
#include "HookStrategy.h"
#include <list>

class AgentConfiguration
{
public:
	AgentConfiguration();
	~AgentConfiguration();

	std::list<MethodSensor*> CreateAndGetMethodSensors();
	HookStrategy* CreateAndGetHookStrategy();

};

