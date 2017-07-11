#pragma once

#include "basehdr.h"
#include "MethodSensorTypeConfig.h"
#include "DataSendingService.h"

/*
 * Interface for method sensors.
 */
class MethodHook {

private:
	JAVA_LONG sensorTypeId;
	JAVA_LONG platformId;
	MethodSensorPriority priority = MIN;

	std::shared_ptr<DataSendingService> dataSendingService;

protected:
	virtual void init(ICorProfilerInfo *profilerInfo) = 0;

	std::shared_ptr<DataSendingService> getDataSendingService();

public:
	MethodHook() {}
	virtual ~MethodHook() {}

	MethodSensorPriority getPriority();
	void setPriority(MethodSensorPriority priority);

	void initialize(ICorProfilerInfo *profilerInfo, std::shared_ptr<DataSendingService> dataSendingService);
	virtual void notifyShutdown() = 0;

	virtual void beforeBody(METHOD_ID methodID) = 0;
	virtual void afterBody(METHOD_ID methodID) = 0;

	void setSensorTypeId(JAVA_LONG sensorTypeId);
	JAVA_LONG getSensorTypeId();

	void setPlatformId(JAVA_LONG platformId);
	JAVA_LONG getPlatformId();
};
