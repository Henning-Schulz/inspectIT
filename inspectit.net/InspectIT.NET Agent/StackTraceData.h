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

	bool isFinished = false;
	std::vector<std::shared_ptr<StackTraceSample>> traces;

protected:
	LPWSTR getJavaTypeName();

public:
	StackTraceData(std::vector<std::shared_ptr<StackTraceSample>> traces, JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, ThreadID threadId, JAVA_LONG timestamp = -1);
	~StackTraceData();

	ThreadID getThreadId();

	std::vector<std::shared_ptr<StackTraceSample>> getTraces();

	web::json::value toJson();
};

