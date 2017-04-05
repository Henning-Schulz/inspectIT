#include "SensorInstrumentationPoint.h"



SensorInstrumentationPoint::SensorInstrumentationPoint()
{
}


SensorInstrumentationPoint::~SensorInstrumentationPoint()
{
}

JAVA_LONG SensorInstrumentationPoint::getId()
{
	return id;
}

std::vector<JAVA_LONG> SensorInstrumentationPoint::getSensorIds()
{
	return sensorIds;
}

std::vector<byte> SensorInstrumentationPoint::getSensorPriorities()
{
	return sensorPriorities;
}

bool SensorInstrumentationPoint::isConstructor()
{
	return constructor;
}

bool SensorInstrumentationPoint::doesStartInvocation()
{
	return startsInvocation;
}

std::map<std::wstring, json::object> SensorInstrumentationPoint::getSettings()
{
	return settings;
}

void SensorInstrumentationPoint::fromJson(json::object json)
{
	id = json.at(L"id").as_number().to_int64();

	json::array sensorArray = json.at(L"sensorIds").as_array();
	for (auto it = sensorArray.begin(); it != sensorArray.end(); it++) {
		sensorIds.push_back(it->as_number().to_int64());
	}

	// sensorPriorities has to be calculated (is not sent)

	constructor = json.at(L"constructor").as_bool();
	startsInvocation = json.at(L"startsInvocation").as_bool();

	json::object settingsMap = json.at(L"settings").as_object();
	for (auto it = settingsMap.begin(); it != settingsMap.end(); it++) {
		settings.emplace(it->first, it->second.as_object());
	}
}
