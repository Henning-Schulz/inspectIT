#pragma once

#include "HookStrategy.h"

class NoHookStrategy : public HookStrategy
{
public:
	NoHookStrategy();
	~NoHookStrategy();

	bool hook(FunctionID functionID, LPWSTR className, LPWSTR methodName);
};

