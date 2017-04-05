#pragma once

#include "basehdr.h"
#include "JsonSendable.h"

#include <vector>
#include <map>

using namespace web;

class MethodSensorMap : JsonSendable
{
private:
	std::map<JAVA_LONG, std::vector<JAVA_LONG>> methodsToSensors;

public:
	MethodSensorMap();
	~MethodSensorMap();

	void addSensor(JAVA_LONG methodId, JAVA_LONG sensorId);
	void addAllSensors(JAVA_LONG methodId, std::vector<JAVA_LONG> sensorIds);

	std::vector<JAVA_LONG> getSensors(JAVA_LONG methodId);

	json::value toJson();
};

