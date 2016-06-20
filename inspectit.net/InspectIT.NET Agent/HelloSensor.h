#pragma once

#include "MethodHook.h"
#include "Logger.h"

class HelloSensor :	public MethodHook
{
private:
	Logger logger = loggerFactory.createLogger("HelloSensor");

public:
	HelloSensor();
	~HelloSensor();

	void init(ICorProfilerInfo *profilerInfo) {}
	void notifyShutdown() {}

	void beforeBody(METHOD_ID methodID);
	void afterBody(METHOD_ID methodID);

};

