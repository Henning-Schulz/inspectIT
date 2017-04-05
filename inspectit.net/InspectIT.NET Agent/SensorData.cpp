#include "SensorData.h"


JAVA_LONG SensorData::getPlatformId()
{
	return platformId;
}

JAVA_LONG SensorData::getSensorTypeId()
{
	return sensorTypeId;
}

JAVA_LONG SensorData::getTimestampMs()
{
	return timestampMs;
}

json::value SensorData::toJson()
{
	json::value json;
	json[KEY_TYPE] = json::value::string(typeName());
	json[L"timestampMs"] = json::value::number(timestampMs);
	return json;
}
