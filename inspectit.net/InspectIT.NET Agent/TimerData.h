#pragma once

#include "MethodSensorData.h"

class TimerData :
	public MethodSensorData
{
private:
	typedef MethodSensorData super;

	JAVA_INT threadId;

	JAVA_LONG startTime;
	JAVA_LONG endTime;

protected:
	std::wstring typeName();

public:
	TimerData(JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, JAVA_INT threadId, JAVA_LONG timestamp, JAVA_LONG startTime, JAVA_LONG endTime);
	~TimerData();

	web::json::value toJson();
};

