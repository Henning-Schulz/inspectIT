#pragma once

#include "MethodHook.h"
#include "ThreadHook.h"
#include "MethodSensorTypeConfig.h"
#include "DataSendingService.h"

#include <memory>

class MethodSensor
{

private:
	JAVA_LONG sensorTypeId;
	JAVA_LONG platformId;
	std::wstring name;

	std::shared_ptr<DataSendingService> dataSendingService;

protected:
	virtual void init(ICorProfilerInfo *profilerInfo) = 0;

	std::shared_ptr<DataSendingService> getDataSendingService();

public:
	MethodSensor() {}
	virtual ~MethodSensor() {}

	void initialize(std::shared_ptr<MethodSensorTypeConfig> config, JAVA_LONG platformId, ICorProfilerInfo *profilerInfo, std::shared_ptr<DataSendingService> dataSendingService);
	virtual void notifyStartup() = 0;
	virtual void notifyShutdown() = 0;

	virtual bool hasHook() = 0;
	virtual std::shared_ptr<MethodHook> getHook() = 0;

	virtual bool hasThreadHook() = 0;
	virtual std::shared_ptr<ThreadHook> getThreadHook() = 0;

	virtual DWORD getSpecialMonitorFlags() = 0;

	virtual std::wstring getClassName() = 0;

	JAVA_LONG getSensorTypeId();
	JAVA_LONG getPlatformId();
	std::wstring getName();
};