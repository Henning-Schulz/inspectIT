#include "MethodSensorMap.h"



MethodSensorMap::MethodSensorMap()
{
}


MethodSensorMap::~MethodSensorMap()
{
}

void MethodSensorMap::addSensor(JAVA_LONG methodId, JAVA_LONG sensorId)
{
	auto it = methodsToSensors.find(methodId);

	if (it == methodsToSensors.end()) {
		std::vector<JAVA_LONG> sensorIds;
		sensorIds.push_back(sensorId);
		methodsToSensors.emplace(methodId, sensorIds);
	}
	else {
		it->second.push_back(sensorId);
	}
}

void MethodSensorMap::addAllSensors(JAVA_LONG methodId, std::vector<JAVA_LONG> sensorIds)
{
	auto it = methodsToSensors.find(methodId);

	if (it == methodsToSensors.end()) {
		methodsToSensors.emplace(methodId, sensorIds);
	}
	else {
		for (auto sit = sensorIds.begin(); sit != sensorIds.end(); sit++) {
			it->second.push_back(*sit);
		}
	}
}

std::vector<JAVA_LONG> MethodSensorMap::getSensors(JAVA_LONG methodId)
{
	auto it = methodsToSensors.find(methodId);

	if (it == methodsToSensors.end()) {
		return std::vector<JAVA_LONG>();
	}
	else {
		return it->second;
	}
}

json::value MethodSensorMap::toJson()
{
	json::value json;

	for (auto oit = methodsToSensors.begin(); oit != methodsToSensors.end(); oit++) {
		json::value inner;

		int i = 0;
		for (auto iit = oit->second.begin(); iit != oit->second.end(); iit++) {
			inner[i] = json::value::number(*iit);
			i++;
		}

		json[std::to_wstring(oit->first)] = inner;
	}

	return json;
}
