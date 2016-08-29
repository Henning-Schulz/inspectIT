#pragma once
#include "MeasurementStorage.h"

#include <chrono>

class TimerStorage :
	public MeasurementStorage
{
private:
	ThreadID threadId;

	JAVA_LONG startNanos = -1;
	JAVA_LONG endNanos = -1;

public:
	TimerStorage(JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, ThreadID threadId, std::chrono::duration<long long, std::nano> startNanos);
	~TimerStorage();

	void setEndTime(std::chrono::duration<long long, std::nano> endNanos);

	bool finished();
	std::shared_ptr<MethodSensorData> finalizeData();
};

