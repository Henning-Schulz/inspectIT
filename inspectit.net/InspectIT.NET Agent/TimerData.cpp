#include "TimerData.h"

using namespace inspectit::sensor::timer;

std::wstring TimerData::typeName()
{
	return L"TimerData";
}

TimerData::TimerData(JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, JAVA_INT threadId, JAVA_LONG timestamp, JAVA_LONG startTime, JAVA_LONG endTime)
	: MethodSensorData(platformId, methodSensorId, methodId, timestamp)
{
	this->threadId = threadId;
	this->startTime = startTime;
	this->endTime = endTime;
}


TimerData::~TimerData()
{
}

web::json::value TimerData::toJson()
{
	web::json::value json = super::toJson();

	json[L"threadId"] = web::json::value::number(threadId);
	json[L"startTime"] = web::json::value::number(startTime);
	json[L"endTime"] = web::json::value::number(endTime);

	return json;
}
