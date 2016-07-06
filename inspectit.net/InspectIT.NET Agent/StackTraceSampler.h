#pragma once

#include "StackTraceProvider.h"
#include "StackTraceStorage.h"
#include "DataSendingService.h"
#include "Logger.h"

#include <memory>
#include <set>

class StackTraceSampler
{
private:
	/* ID of the sensor providing the data (e.g. of ShadowStackHook) */
	JAVA_LONG sensorTypeId;
	JAVA_LONG platformId;

	std::shared_ptr<StackTraceProvider> provider;

	std::set<ThreadID> allThreads;

	Logger logger = loggerFactory.createLogger("StackTraceSampler");

	void storeTrace(ThreadID threadId, std::shared_ptr<StackTraceSample> trace, bool forceNewStorage);

public:
	StackTraceSampler(std::shared_ptr<StackTraceProvider> provider);
	~StackTraceSampler();

	void setSensorTypeId(JAVA_LONG sensorTypeId);
	void setPlatformId(JAVA_LONG platformId);

	/*
	 * Should be called by the trigger
	 */
	void doSampling(ThreadID threadId, bool forceNewStorage);
	void doSamplingOfAllThreads(bool forceNewStorage);

	void notifyShutdown();
};

