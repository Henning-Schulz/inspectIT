#pragma once

#include "MethodHook.h"
#include "HookStrategy.h"
#include <list>

class AgentConfiguration
{
public:
	AgentConfiguration();
	~AgentConfiguration();

	std::list<MethodHook*> CreateAndGetMethodSensors();
	HookStrategy* CreateAndGetHookStrategy();

};

