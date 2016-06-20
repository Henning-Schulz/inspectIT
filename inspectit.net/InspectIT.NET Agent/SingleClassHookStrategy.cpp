#include "SingleClassHookStrategy.h"

#include <vector>


SingleClassHookStrategy::SingleClassHookStrategy(LPWSTR className)
{
	this->className = className;
}


SingleClassHookStrategy::~SingleClassHookStrategy()
{
}

bool SingleClassHookStrategy::hook(LPWSTR className, LPWSTR methodName, std::vector<LPWSTR> parameters, JAVA_INT modifiers)
{
	return wcscmp(this->className, className) == 0;
}