#include "SingleClassHookStrategy.h"



SingleClassHookStrategy::SingleClassHookStrategy(LPWSTR className)
{
	this->className = className;
}


SingleClassHookStrategy::~SingleClassHookStrategy()
{
}

bool SingleClassHookStrategy::hook(FunctionID functionID, LPWSTR className, LPWSTR methodName)
{
	return wcscmp(this->className, className) == 0;
}