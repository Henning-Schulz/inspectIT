#pragma once
#include "SamplingTrigger.h"

#include <random>

class RandomizedTimedTrigger :
	public SamplingTrigger
{
private:
	Logger logger = loggerFactory.createLogger("RandomizedTimedTrigger");

	ThreadID threadId;

	std::shared_ptr<StackTraceSampler> sampler;
	std::thread worker;
	std::chrono::duration<long long, std::milli> retryInterval;
	int maxNumberOfRetries;
	std::mutex sleepMutex;
	std::condition_variable sleepCV;

	int samplingInterval;
	std::default_random_engine randomGenerator;
	std::uniform_int_distribution<int> samplingDistribution;

	// In contrast to bool, 32 bit integer has atomic read and write operations
	int32_t stopped = 0;

	METHOD_ID baseMethodId;
	JAVA_LONG startTime;

	void workerMethod();

public:
	RandomizedTimedTrigger(ThreadID threadId, JAVA_LONG samplingInterval);
	~RandomizedTimedTrigger();

	void start(std::shared_ptr<StackTraceSampler> sampler, METHOD_ID baseMethodId = 0, JAVA_LONG startTime = -1);
	void stop(JAVA_LONG stopTime = -1);

	std::shared_ptr<StackTraceSampler> getSampler();
};

