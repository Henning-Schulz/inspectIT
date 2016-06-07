#include "NoHookStrategy.h"



NoHookStrategy::NoHookStrategy()
{
}


NoHookStrategy::~NoHookStrategy()
{
}

bool NoHookStrategy::hook(FunctionID functionID, LPWSTR className, LPWSTR methodName)
{
	return false;
}