#include "TimerData.h"

using namespace inspectit::sensor::timer;

std::wstring TimerData::typeName()
{
	return L"TimerData";
}

TimerData::TimerData(JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, JAVA_INT threadId, JAVA_LONG timestamp)
	: MethodSensorData(platformId, methodSensorId, methodId, timestamp)
{
	this->threadId = threadId;
}


TimerData::~TimerData()
{
}

void inspectit::sensor::timer::TimerData::setStartTime(JAVA_LONG startTime)
{
	this->startTime = startTime;
}

void inspectit::sensor::timer::TimerData::setEndTime(JAVA_LONG endTime)
{
	this->endTime = endTime;
}

web::json::value TimerData::toJson()
{
	web::json::value json = super::toJson();

	json[L"threadId"] = web::json::value::number(threadId);
	json[L"startTime"] = web::json::value::number(startTime);
	json[L"endTime"] = web::json::value::number(endTime);

	return json;
}
