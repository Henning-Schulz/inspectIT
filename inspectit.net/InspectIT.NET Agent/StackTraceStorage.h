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

	bool isFinished = false;
	std::vector<std::shared_ptr<StackTraceSample>> traces;

public:
	StackTraceStorage(JAVA_LONG platformId, JAVA_LONG methodSensorId, ThreadID threadId, JAVA_LONG timestamp = -1);
	~StackTraceStorage();

	ThreadID getThreadId();

	std::vector<std::shared_ptr<StackTraceSample>> getTraces();
	JAVA_LONG getHighestMethod();

	void addTrace(std::shared_ptr<StackTraceSample> trace);

	void finish();
	bool finished();
	std::shared_ptr<MethodSensorData> finalizeData();
};

