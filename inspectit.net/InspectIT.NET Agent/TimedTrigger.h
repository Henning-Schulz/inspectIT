#pragma once

#include "SamplingTrigger.h"
#include "Logger.h"

#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include <mutex>
#include <condition_variable>

class TimedTrigger : public SamplingTrigger
{
private:
	Logger logger = loggerFactory.createLogger("TimedTrigger");

	ThreadID threadId;

	std::shared_ptr<StackTraceSampler> sampler;
	std::thread worker;
	std::chrono::duration<long long, std::milli> samplingInterval;
	std::chrono::duration<long long, std::milli> retryInterval;
	int maxNumberOfRetries;
	std::mutex sleepMutex;
	std::condition_variable sleepCV;

	// In contrast to bool, 32 bit integer has atomic read and write operations
	int32_t stopped = 0;

	METHOD_ID baseMethodId;
	JAVA_LONG startTime;

	void workerMethod();

public:
	TimedTrigger(ThreadID threadId, JAVA_LONG samplingInterval);
	~TimedTrigger();

	std::shared_ptr<StackTraceSampler> getSampler();

	void start(std::shared_ptr<StackTraceSampler> sampler, METHOD_ID baseMethodId = 0, JAVA_LONG startTime = -1);
	void stop(JAVA_LONG stopTime = -1);
};

