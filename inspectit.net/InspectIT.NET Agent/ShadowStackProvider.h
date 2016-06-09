#pragma once

#include "ShadowStack.h"
#include "StackTraceProvider.h"
#include "Logger.h"

#include <map>
#include <memory>
#include <shared_mutex>

class ShadowStackProvider : public StackTraceProvider
{
private:
	std::map<ThreadID, std::shared_ptr<ShadowStack>> stacksPerThread;
	std::shared_mutex stacksMutex;

	Logger logger = loggerFactory.createLogger("ShadowStackProvider");

public:
	ShadowStackProvider();
	~ShadowStackProvider();

	void pushMethod(ThreadID threadId, METHOD_ID methodId);
	void popMethod(ThreadID threadId);

	std::shared_ptr<StackTraceSample> getStackTrace(ThreadID threadId);
	std::map<ThreadID, std::shared_ptr<StackTraceSample>> getAllStackTraces();
};

