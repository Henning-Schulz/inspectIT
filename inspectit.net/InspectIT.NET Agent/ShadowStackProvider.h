#pragma once

#include "ShadowStack.h"
#include "StackTraceProvider.h"
#include "ShadowStackHolder.h"
#include "ShadowStackHook.h"
#include "Logger.h"

#include <map>
#include <memory>
#include <shared_mutex>

class ShadowStackProvider : public StackTraceProvider, public ShadowStackHolder
{
private:
	std::map<ThreadID, std::shared_ptr<ShadowStack>> stacksPerThread;
	std::shared_mutex stacksMutex;

	std::shared_ptr<ShadowStackHook> hook;

	Logger logger = loggerFactory.createLogger("ShadowStackProvider");

public:
	ShadowStackProvider();
	~ShadowStackProvider();

	bool hasHook();
	std::shared_ptr<MethodHook> getHook();

	bool hasThreadHook();
	std::shared_ptr<ThreadHook> getThreadHook();

	/*
	* Returns necessary monitor flags except for COR_PRF_MONITOR_ENTERLEAVE
	*/
	DWORD getSpecialMonitorFlags();

	void pushMethod(ThreadID threadId, METHOD_ID methodId);
	void popMethod(ThreadID threadId);

	std::shared_ptr<StackTraceSample> getStackTrace(ThreadID threadId);
	std::map<ThreadID, std::shared_ptr<StackTraceSample>> getAllStackTraces();
};

