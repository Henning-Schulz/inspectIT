#pragma once

#include "MethodSensorData.h"
#include "StackTraceSample.h"

#include <vector>
#include <memory>

class StackTraceData : public MethodSensorData
{
private:
	typedef MethodSensorData super;

	ThreadID threadId;

	JAVA_LONG baseMethodId = 0;
	JAVA_LONG startTime = -1;
	JAVA_LONG endTime = -1;

	std::vector<std::shared_ptr<StackTraceSample>> traces;

protected:
	LPWSTR getJavaTypeName();

public:
	StackTraceData(std::vector<std::shared_ptr<StackTraceSample>> traces, JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, ThreadID threadId, JAVA_LONG timestamp = -1);
	~StackTraceData();

	ThreadID getThreadId();

	JAVA_LONG getBaseMethodId();
	void setBaseMethodId(JAVA_LONG baseMethodId);
	JAVA_LONG getStartTime();
	void setStartTime(JAVA_LONG startTime);
	JAVA_LONG getEndTime();
	void setEndTime(JAVA_LONG endTime);

	std::vector<std::shared_ptr<StackTraceSample>> getTraces();

	web::json::value toJson();
};

