#pragma once
#include "basehdr.h"
#include "JsonReceivable.h"

#include <map>

class AbstractSensorTypeConfig :
	public JsonReceivable
{
private:
	JAVA_LONG id;
	std::wstring className;

	std::map<std::wstring, json::object> parameters;

public:
	AbstractSensorTypeConfig();
	~AbstractSensorTypeConfig();

	JAVA_LONG getId();
	std::wstring getClassName();

	std::map<std::wstring, json::object> getParameters();

	void fromJson(json::object json);
};

