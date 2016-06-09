#include "StackTraceData.h"



StackTraceData::StackTraceData(std::vector<std::shared_ptr<StackTraceSample>> traces, JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, ThreadID threadId, std::time_t timestamp)
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
	
	web::json::value tracesJson;
	int i = 0;
	for (auto it = traces.begin(); it != traces.end(); it++) {
		tracesJson[i] = (*it)->toJson();
		i++;
	}

	json[L"traces"] = tracesJson;

	return json;
}