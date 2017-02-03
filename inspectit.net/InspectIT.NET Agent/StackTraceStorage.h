#pragma once

#include "MeasurementStorage.h"
#include "StackTraceSample.h"
#include "StackTraceData.h"

#include <vector>
#include <memory>

class StackTraceStorage : public MeasurementStorage
{
private:
	typedef MeasurementStorage super;

	ThreadID threadId;

	METHOD_ID baseMethodId = 0;
	JAVA_LONG startTime = -1;
	JAVA_LONG endTime = -1;

	bool isFinished = false;
	std::vector<std::shared_ptr<StackTraceSample>> traces;

public:
	StackTraceStorage(JAVA_LONG platformId, JAVA_LONG methodSensorId, ThreadID threadId, JAVA_LONG timestamp = -1);
	~StackTraceStorage();

	ThreadID getThreadId();

	METHOD_ID getBaseMethodId();
	void setBaseMethodId(METHOD_ID baseMethodId);
	JAVA_LONG getStartTime();
	void setStartTime(JAVA_LONG startTime);
	JAVA_LONG getEndTime();
	void setEndTime(JAVA_LONG endTime);

	std::vector<std::shared_ptr<StackTraceSample>> getTraces();
	JAVA_LONG getHighestMethod();

	void addTrace(std::shared_ptr<StackTraceSample> trace);

	void finish();
	bool finished();
	std::vector<std::shared_ptr<MethodSensorData>> finalizeData();
};

