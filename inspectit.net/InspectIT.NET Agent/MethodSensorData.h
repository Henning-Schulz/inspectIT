#pragma once

#include "basehdr.h"

#include <cpprest/json.h>
#include <chrono>

#define TYPE_KEY L"objectType"

class MethodSensorData
{
private:
	// Java long has 64 bits; C++ std::time_t = long long (normally) as well
	JAVA_LONG timestamp;
	JAVA_LONG platformId;
	JAVA_LONG methodSensorId;
	JAVA_LONG methodId;

protected:
	virtual LPWSTR getJavaTypeName() = 0;

public:
	MethodSensorData(JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, JAVA_LONG timestamp = -1)
	{
		this->platformId = platformId;
		this->methodSensorId = methodSensorId;
		this->methodId = methodId;

		if (timestamp < 0) {
			this->timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		}
		else {
			this->timestamp = timestamp;
		}
	}
	virtual ~MethodSensorData() {}

	JAVA_LONG getTimestamp() { return timestamp; }
	JAVA_LONG getPlatformId() { return platformId; }
	JAVA_LONG getMethodSensorId() { return methodSensorId; }
	JAVA_LONG getMethodId() { return methodId; }

	virtual web::json::value toJson()
	{
		web::json::value json;
		json[TYPE_KEY] = web::json::value::string(getJavaTypeName());
		json[L"timestamp"] = web::json::value::number(getTimestamp());
		json[L"platformId"] = web::json::value::number(getPlatformId());
		json[L"methodSensorId"] = web::json::value::number(getMethodSensorId());
		json[L"methodId"] = web::json::value::number(getMethodId());
		return json;
	}

};