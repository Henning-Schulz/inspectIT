#pragma once

#include "basehdr.h"

class HookStrategy {

public:
	HookStrategy() {}
	virtual ~HookStrategy() {}

	virtual bool hook(FunctionID functionID, LPWSTR className, LPWSTR methodName) = 0;
};