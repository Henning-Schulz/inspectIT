#pragma once

#include "basehdr.h"
#include "MethodSensorData.h"

#include <memory>

class MeasurementStorage
{
private:
	JAVA_LONG timestamp;
	JAVA_LONG platformId;
	JAVA_LONG methodSensorId;
	JAVA_LONG methodId;

public:
	MeasurementStorage(JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, std::time_t timestamp = time(nullptr))
	{
		this->platformId = platformId;
		this->methodSensorId = methodSensorId;
		this->methodId = methodId;
		this->timestamp = timestamp;
	}

	virtual ~MeasurementStorage() {}

	JAVA_LONG getTimestamp() { return timestamp; }
	JAVA_LONG getPlatformId() { return platformId; }
	JAVA_LONG getMethodSensorId() { return methodSensorId; }
	JAVA_LONG getMethodId() { return methodId; }

	virtual bool finished() = 0;
	virtual std::shared_ptr<MethodSensorData> finalizeData() = 0;
};
