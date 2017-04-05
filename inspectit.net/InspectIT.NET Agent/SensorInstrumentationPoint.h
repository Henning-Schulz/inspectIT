#pragma once

#include "basehdr.h"
#include "JsonReceivable.h"

#include <map>

class SensorInstrumentationPoint :
	public JsonReceivable
{
private:
	JAVA_LONG id;
	std::vector<JAVA_LONG> sensorIds;
	std::vector<byte> sensorPriorities;
	bool constructor;
	bool startsInvocation;
	std::map<std::wstring, json::object> settings;

	// TODO: propertyAccessorList

public:
	SensorInstrumentationPoint();
	~SensorInstrumentationPoint();

	JAVA_LONG getId();
	std::vector<JAVA_LONG> getSensorIds();
	std::vector<byte> getSensorPriorities();
	bool isConstructor();
	bool doesStartInvocation();
	std::map<std::wstring, json::object> getSettings();

	void fromJson(json::object json);
};

