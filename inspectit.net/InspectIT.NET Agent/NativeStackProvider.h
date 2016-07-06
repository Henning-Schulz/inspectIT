#pragma once

#include "StackTraceProvider.h"
#include "Logger.h"
#include "ThreadHook.h"

#include <vector>
#include <map>
#include <shared_mutex>

class NativeStackProvider : public StackTraceProvider, public ThreadHook
{
private:
	ICorProfilerInfo2 *profilerInfo;

	Logger logger = loggerFactory.createLogger("NativeStackProvider");

	std::vector<ThreadID> createdThreads;
	std::shared_mutex mCreatedThreads;

	std::map<FunctionID, METHOD_ID> functionToMethodId;

	int numSamplingErrors = 0;
	std::shared_mutex mNumSamplingErrors;

	void addSamplingError();
	void printNumSamplingErrors();

	METHOD_ID getMethodId(FunctionID functionId);

public:
	NativeStackProvider(ICorProfilerInfo2 *profilerInfo);
	~NativeStackProvider();

	bool hasHook();
	std::shared_ptr<MethodHook> getHook();

	bool hasThreadHook();
	std::shared_ptr<ThreadHook> getThreadHook();

	void threadCreated(ThreadID threadId, bool delayed);
	void threadDestroyed(ThreadID threadId, bool delayed);

	/*
	* Returns necessary monitor flags except for COR_PRF_MONITOR_ENTERLEAVE
	*/
	DWORD getSpecialMonitorFlags();

	void nextStackFrame(FunctionID functionId, std::vector<JAVA_LONG> *trace);

	std::shared_ptr<StackTraceSample> getStackTrace(ThreadID threadId);
	std::map<ThreadID, std::shared_ptr<StackTraceSample>> getAllStackTraces();
};

extern std::shared_ptr<NativeStackProvider> providerInstance;