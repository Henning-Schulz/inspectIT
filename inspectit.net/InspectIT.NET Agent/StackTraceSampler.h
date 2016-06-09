#pragma once

#include "StackTraceProvider.h"
#include "StackTraceStorage.h"
#include "DataSendingService.h"
#include "Logger.h"

#include <memory>

class StackTraceSampler
{
private:
	/* ID of the sensor providing the data (e.g. of ShadowStackSensor) */
	JAVA_LONG sensorTypeId;
	JAVA_LONG platformId;

	std::shared_ptr<StackTraceProvider> provider;

	Logger logger = loggerFactory.createLogger("StackTraceSampler");

	void storeTrace(ThreadID threadId, std::shared_ptr<StackTraceSample> trace, bool forceNewStorage);

public:
	StackTraceSampler(std::shared_ptr<StackTraceProvider> provider, JAVA_LONG sensorTypeId, JAVA_LONG platformId);
	~StackTraceSampler();

	/*
	 * Should be called by the trigger
	 */
	void doSampling(ThreadID threadId, bool forceNewStorage);
	void doSamplingOfAllThreads(bool forceNewStorage);
};

