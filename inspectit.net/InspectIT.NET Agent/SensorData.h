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
	SensorData(JAVA_LONG platformId, JAVA_LONG sensorTypeId, JAVA_LONG timestampMs = -1);
	virtual ~SensorData() {}

	JAVA_LONG getPlatformId();
	JAVA_LONG getSensorTypeId();
	JAVA_LONG getTimestampMs();

	virtual JAVA_LONG getIdentifyingId() = 0;

	virtual json::value toJson();
};

