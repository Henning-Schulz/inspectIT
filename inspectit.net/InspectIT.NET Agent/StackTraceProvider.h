#pragma once

#include "StackTraceSample.h"
#include "MethodHook.h"
#include "ThreadHook.h"
#include "HookStrategy.h"

#include <memory>
#include <map>

class StackTraceProvider
{
public:
	StackTraceProvider() {}
	virtual ~StackTraceProvider() {}

	virtual void init() = 0;

	virtual bool hasHook() = 0;
	virtual std::shared_ptr<MethodHook> getHook() = 0;

	virtual void setHookStrategy(std::shared_ptr<HookStrategy> hookStrategy) = 0;


	/*
	 * Returns necessary monitor flags except for COR_PRF_MONITOR_ENTERLEAVE
	 */
	virtual DWORD getSpecialMonitorFlags() = 0;

	virtual std::shared_ptr<StackTraceSample> getStackTrace(ThreadID threadId) = 0;
};