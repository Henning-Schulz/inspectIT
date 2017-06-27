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
	logger.debug("1");
	super::fromJson(json);
	
	logger.debug("2");
	name = json.at(L"name").as_string();
	logger.debug("3");
	std::wstring priorityName = json.at(L"priority").as_string();
	logger.debug("4");
	priority = prioFromString(priorityName);
	logger.debug("5");
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