#include "AgentConfiguration.h"



AgentConfiguration::AgentConfiguration()
{
}


AgentConfiguration::~AgentConfiguration()
{
}

std::list<MethodHook*> AgentConfiguration::CreateAndGetMethodSensors()
{
	return std::list<MethodHook*>();
}

HookStrategy* AgentConfiguration::CreateAndGetHookStrategy()
{
	return NULL;
}