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
	MeasurementStorage(JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, JAVA_LONG timestamp = -1)
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

	virtual ~MeasurementStorage() {}

	JAVA_LONG getTimestamp() { return timestamp; }
	JAVA_LONG getPlatformId() { return platformId; }
	JAVA_LONG getMethodSensorId() { return methodSensorId; }
	JAVA_LONG getMethodId() { return methodId; }

	virtual bool finished() = 0;
	virtual std::shared_ptr<MethodSensorData> finalizeData() = 0;
};
