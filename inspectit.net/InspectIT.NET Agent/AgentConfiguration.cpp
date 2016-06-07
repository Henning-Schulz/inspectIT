#include "AgentConfiguration.h"



AgentConfiguration::AgentConfiguration()
{
}


AgentConfiguration::~AgentConfiguration()
{
}

std::list<MethodSensor*> AgentConfiguration::CreateAndGetMethodSensors()
{
	return std::list<MethodSensor*>();
}

HookStrategy* AgentConfiguration::CreateAndGetHookStrategy()
{
	return NULL;
}