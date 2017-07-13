#include "SensorData.h"

using namespace inspectit::data;

SensorData::SensorData(JAVA_LONG platformId, JAVA_LONG sensorTypeId, JAVA_LONG timestampMs)
{
	this->platformId = platformId;
	this->sensorTypeId = sensorTypeId;

	if (timestampMs < 0) {
		this->timestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	else {
		this->timestampMs = timestampMs;
	}
}

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
	web::json::value json;
	json[KEY_TYPE] = web::json::value::string(typeName());
	json[L"timestampMs"] = web::json::value::number(timestampMs);
	return json;
}
