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

	void init(std::shared_ptr<MethodSensorConfig> config, JAVA_LONG sensorTypeId, JAVA_LONG platformId, ICorProfilerInfo *profilerInfo);
	void notifyStartup();
	void notifyShutdown();

	bool hasHook();
	std::shared_ptr<MethodHook> getHook();

	bool hasThreadHook();
	std::shared_ptr<ThreadHook> getThreadHook();

	DWORD getSpecialMonitorFlags();
};

