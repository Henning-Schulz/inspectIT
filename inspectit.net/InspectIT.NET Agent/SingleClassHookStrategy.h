#pragma once

#include "HookStrategy.h"

class SingleClassHookStrategy : public HookStrategy
{
private:
	LPWSTR className;

public:
	SingleClassHookStrategy(LPWSTR className);
	~SingleClassHookStrategy();

	bool hook(FunctionID functionID, LPWSTR className, LPWSTR methodName);
};

