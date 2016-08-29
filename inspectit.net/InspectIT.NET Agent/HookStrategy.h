#pragma once

#include "basehdr.h"
#include "MethodHook.h"

#include <vector>

class HookStrategy {

public:
	HookStrategy() {}
	virtual ~HookStrategy() {}

	virtual bool hook(LPWSTR className, LPWSTR methodName, std::vector<LPWSTR> parameters, JAVA_INT modifiers) = 0;
};