#pragma once
#include "MethodSensor.h"

#include "TimerHook.h"

class TimerSensor :
	public MethodSensor
{
private:
	std::shared_ptr<TimerHook> hook;

public:
	TimerSensor();
	~TimerSensor();

	void init(ICorProfilerInfo *profilerInfo);
	void notifyStartup();
	void notifyShutdown();

	bool hasHook();
	std::shared_ptr<MethodHook> getHook();

	bool hasThreadHook();
	std::shared_ptr<ThreadHook> getThreadHook();

	DWORD getSpecialMonitorFlags();

	std::wstring getClassName();
};

