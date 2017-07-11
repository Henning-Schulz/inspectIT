#pragma once

#include "MethodHook.h"
#include "Logger.h"

class TimerHook : public MethodHook
{
private:
	Logger logger = loggerFactory.createLogger("TimerHook");

	ICorProfilerInfo *profilerInfo;

protected:
	void init(ICorProfilerInfo *profilerInfo);

public:
	TimerHook();
	~TimerHook();
	void notifyShutdown();

	void beforeBody(METHOD_ID methodID);
	void afterBody(METHOD_ID methodID);
};

