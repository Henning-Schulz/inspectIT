#pragma once

#include "ShadowStack.h"
#include "StackTraceProvider.h"
#include "ShadowStackHolder.h"
#include "ShadowStackHook.h"
#include "Logger.h"

#include <map>
#include <memory>
#include <shared_mutex>

class ShadowStackProvider : public StackTraceProvider, public ShadowStackHolder,
	public std::enable_shared_from_this<ShadowStackProvider>
{
private:
	std::map<ThreadID, std::shared_ptr<ShadowStack>> stacksPerThread;
	std::shared_mutex stacksMutex;

	std::shared_ptr<ShadowStackHook> hook;

	Logger logger = loggerFactory.createLogger("ShadowStackProvider");

public:
	ShadowStackProvider();
	~ShadowStackProvider();

	void init();

	void setHookStrategy(std::shared_ptr<HookStrategy> hookStrategy) {}

	bool hasHook();
	std::shared_ptr<MethodHook> getHook();

	/*
	* Returns necessary monitor flags except for COR_PRF_MONITOR_ENTERLEAVE
	*/
	DWORD getSpecialMonitorFlags();

	void pushMethod(ThreadID threadId, METHOD_ID methodId);
	void popMethod(ThreadID threadId);

	std::shared_ptr<StackTraceSample> getStackTrace(ThreadID threadId);

	void addShadowStackListener(ThreadID threadId, std::shared_ptr<ShadowStackListener> listener);
};

