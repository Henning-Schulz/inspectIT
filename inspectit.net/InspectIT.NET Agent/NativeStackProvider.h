#pragma once

#include "StackTraceProvider.h"
#include "Logger.h"

#include <vector>
#include <map>
#include <shared_mutex>

class NativeStackProvider : public StackTraceProvider, public std::enable_shared_from_this<NativeStackProvider>
{
private:
	ICorProfilerInfo2 *profilerInfo;

	Logger logger = loggerFactory.createLogger("NativeStackProvider");

	size_t maximumTraceSize;

	std::shared_ptr<HookStrategy> hookStrategy;

	std::map<FunctionID, METHOD_ID> functionToMethodId;

	int numSamplingErrors = 0;
	std::shared_mutex mNumSamplingErrors;

	void addSamplingError();
	void printNumSamplingErrors();

	METHOD_ID getMethodId(FunctionID functionId);

public:
	NativeStackProvider(ICorProfilerInfo2 *profilerInfo);
	~NativeStackProvider();

	void init();

	void setHookStrategy(std::shared_ptr<HookStrategy> hookStrategy);

	bool hasHook();
	std::shared_ptr<MethodHook> getHook();

	/*
	* Returns necessary monitor flags except for COR_PRF_MONITOR_ENTERLEAVE
	*/
	DWORD getSpecialMonitorFlags();

	void nextStackFrame(FunctionID functionId, std::vector<FunctionID> *trace);

	std::shared_ptr<StackTraceSample> getStackTrace(ThreadID threadId);
};

extern std::shared_ptr<NativeStackProvider> providerInstance;