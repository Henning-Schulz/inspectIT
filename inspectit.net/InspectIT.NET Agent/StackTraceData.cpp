#include "StackTraceData.h"

#include <chrono>


StackTraceData::StackTraceData(std::vector<std::shared_ptr<StackTraceSample>> traces, JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, ThreadID threadId, JAVA_LONG timestamp)
	: MethodSensorData(platformId, methodSensorId, methodId, timestamp)
{
	this->threadId = threadId;
	this->traces = traces;
}


StackTraceData::~StackTraceData()
{
}

ThreadID StackTraceData::getThreadId()
{
	return threadId;
}

JAVA_LONG StackTraceData::getBaseMethodId() {
	return baseMethodId;
}

void StackTraceData::setBaseMethodId(JAVA_LONG baseMethodId)
{
	this->baseMethodId = baseMethodId;
}

JAVA_LONG StackTraceData::getStartTime()
{
	return startTime;
}

void StackTraceData::setStartTime(JAVA_LONG startTime)
{
	this->startTime = startTime;
}

JAVA_LONG StackTraceData::getEndTime()
{
	return endTime;
}

void StackTraceData::setEndTime(JAVA_LONG endTime)
{
	this->endTime = endTime;
}

LPWSTR StackTraceData::getJavaTypeName()
{
	return L"StackTraceData";
}

std::vector<std::shared_ptr<StackTraceSample>> StackTraceData::getTraces()
{
	return traces;
}

web::json::value StackTraceData::toJson()
{
	web::json::value json = super::toJson();
	json[L"threadId"] = web::json::value::number(threadId);
	json[L"baseMethodId"] = web::json::value::number(baseMethodId);
	json[L"startTime"] = web::json::value::number(startTime);
	json[L"endTime"] = web::json::value::number(endTime);
	
	web::json::value tracesJson;
	int i = 0;
	for (auto it = traces.begin(); it != traces.end(); it++) {
		tracesJson[i] = (*it)->toJson();
		i++;
	}

	json[L"traces"] = tracesJson;

	return json;
}