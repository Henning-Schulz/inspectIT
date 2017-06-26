#include "MethodSensorTypeConfig.h"



MethodSensorPriority MethodSensorTypeConfig::prioFromString(std::wstring sPrio)
{
	if (sPrio.compare(L"INVOC") == 0) {
		return INVOC;
	} else if (sPrio.compare(L"MIN") == 0) {
		return MIN;
	}
	else if (sPrio.compare(L"LOW") == 0) {
		return LOW;
	}
	else if (sPrio.compare(L"NORMAL") == 0) {
		return NORMAL;
	}
	else if (sPrio.compare(L"HIGH") == 0) {
		return HIGH;
	}
	else if (sPrio.compare(L"MAX") == 0) {
		return MAX;
	}
}

MethodSensorTypeConfig::MethodSensorTypeConfig()
{
}


MethodSensorTypeConfig::~MethodSensorTypeConfig()
{
}

std::wstring MethodSensorTypeConfig::getName()
{
	return name;
}

MethodSensorPriority MethodSensorTypeConfig::getPriority()
{
	return priority;
}

void MethodSensorTypeConfig::fromJson(json::object json)
{
	super::fromJson(json);
	
	name = json.at(L"name").as_string();
	std::wstring priorityName = json.at(L"priority").as_string();
	priority = prioFromString(priorityName);
}

MethodSensorPriority prioFromString(std::wstring sPrio)
{
	if (sPrio.compare(L"INVOC") == 0) {
		return INVOC;
	}
	else if (sPrio.compare(L"MIN") == 0) {
		return MIN;
	}
	else if (sPrio.compare(L"LOW") == 0) {
		return LOW;
	}
	else if (sPrio.compare(L"NORMAL") == 0) {
		return NORMAL;
	}
	else if (sPrio.compare(L"HIGH") == 0) {
		return HIGH;
	}
	else if (sPrio.compare(L"MAX") == 0) {
		return MAX;
	}
}