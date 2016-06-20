#pragma once

#include "HookStrategy.h"

class SingleClassHookStrategy : public HookStrategy
{
private:
	LPWSTR className;

public:
	SingleClassHookStrategy(LPWSTR className);
	~SingleClassHookStrategy();

	bool hook(LPWSTR className, LPWSTR methodName, std::vector<LPWSTR> parameters, JAVA_INT modifiers);
};

