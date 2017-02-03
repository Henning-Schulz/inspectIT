#pragma once

#include "MethodHook.h"
#include "Logger.h"

class TimerHook : public MethodHook
{
private:
	Logger logger = loggerFactory.createLogger("TimerHook");

	ICorProfilerInfo *profilerInfo;

public:
	TimerHook();
	~TimerHook();

	void init(ICorProfilerInfo *profilerInfo);
	void notifyShutdown();

	void beforeBody(METHOD_ID methodID);
	void afterBody(METHOD_ID methodID);
};

