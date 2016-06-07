#pragma once

#include "MethodSensorData.h"
#include "MeasurementStorage.h"

class HelloSensorData : public MethodSensorData, public MeasurementStorage
{
private:
	typedef MethodSensorData super;

	LPWSTR greeting;
	bool bFinished = false;
	int counter = 0;

protected:
	LPWSTR getJavaTypeName();

public:
	HelloSensorData(LPWSTR greeting, JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, std::time_t timestamp = time(nullptr));
	~HelloSensorData();

	LPWSTR getGreeting();

	void addCount();

	web::json::value toJson();

	int getCounter();
	void finish();
	bool finished();
	std::shared_ptr<MethodSensorData> finalizeData();
};

