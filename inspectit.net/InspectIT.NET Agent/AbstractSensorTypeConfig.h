#pragma once
#include "basehdr.h"
#include "JsonReceivable.h"
#include "Logger.h"

#include <map>

class AbstractSensorTypeConfig :
	public JsonReceivable
{
private:
	JAVA_LONG id;
	std::wstring className;

	std::map<std::wstring, std::wstring> parameters;

	Logger logger = loggerFactory.createLogger("AbstractSensorTypeConfig");

public:
	AbstractSensorTypeConfig();
	~AbstractSensorTypeConfig();

	JAVA_LONG getId();
	std::wstring getClassName();

	std::map<std::wstring, std::wstring> getParameters();

	void fromJson(json::object json);
};

