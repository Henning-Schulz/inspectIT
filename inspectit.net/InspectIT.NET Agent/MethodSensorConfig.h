#pragma once

#include "basehdr.h"
#include "cmp_wstr.h"

#include <map>


enum MethodSensorPriority { INVOC = 1, MIN, LOW, NORMAL, HIGH, MAX };
static std::map<const wchar_t*, MethodSensorPriority, cmp_wstr> methodSensorPriorities({ { L"INVOC", INVOC }, { L"MIN", MIN }, { L"LOW", LOW }, { L"NORMAL", NORMAL }, { L"HIGH", HIGH }, { L"MAX", MAX } });

class MethodSensorConfig
{
public:
	MethodSensorConfig() {}
	virtual ~MethodSensorConfig() {}

	virtual LPWSTR getClassName() = 0;
	virtual LPWSTR getName() = 0;
	virtual MethodSensorPriority getPriority() = 0;
};