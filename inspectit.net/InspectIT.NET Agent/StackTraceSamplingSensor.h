#pragma once

#include "MethodSensor.h"
#include "StackTraceSampler.h"
#include "SamplingTrigger.h"
#include "Logger.h"

class StackTraceSamplingSensor : public MethodSensor
{
private:
	Logger logger = loggerFactory.createLogger("StackTraceSamplingSensor");

	std::shared_ptr<StackTraceSampler> sampler;
	std::shared_ptr<SamplingTrigger> trigger;
	std::shared_ptr<StackTraceProvider> provider;

public:
	StackTraceSamplingSensor();
	~StackTraceSamplingSensor();

	void init(std::shared_ptr<MethodSensorConfig> config, JAVA_LONG sensorTypeId, JAVA_LONG platformId, ICorProfilerInfo *profilerInfo);
	void notifyShutdown();

	bool hasHook();
	std::shared_ptr<MethodHook> getHook();
};

