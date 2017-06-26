#pragma once
#include "basehdr.h"
#include "JsonSendable.h"

using namespace web;

#define KEY_TYPE L"dataType"

class SensorData : public JsonSendable
{
private:
	JAVA_LONG platformId;
	JAVA_LONG sensorTypeId;
	JAVA_LONG timestampMs;

protected:
	virtual std::wstring typeName() = 0;


public:
	SensorData() {}
	virtual ~SensorData() {}

	JAVA_LONG getPlatformId();
	JAVA_LONG getSensorTypeId();
	JAVA_LONG getTimestampMs();

	json::value toJson();
};

