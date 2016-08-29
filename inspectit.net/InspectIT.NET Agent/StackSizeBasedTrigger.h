#pragma once
#include "SamplingTrigger.h"
#include "ShadowStackListener.h"

class StackSizeBasedTrigger : public SamplingTrigger, public ShadowStackListener
{
private:
	Logger logger = loggerFactory.createLogger("StackSizeBasedTrigger");

	ThreadID threadId;

	std::shared_ptr<StackTraceSampler> sampler;
	std::thread timerThread;
	std::chrono::duration<long long, std::milli> maxSamplingInterval;
	std::chrono::duration<long long, std::milli> retryInterval;
	int maxNumberOfRetries;

	std::mutex timeoutMutex;
	std::condition_variable timeoutCV;

	std::mutex startSamplingMutex;
	std::condition_variable startSamplingCV;

	std::mutex samplingDoneMutex;
	std::condition_variable samplingDoneCV;

	std::mutex pauseMutex;
	std::condition_variable pauseCV;

	std::mutex pausedMutex;
	std::condition_variable pausedCV;

	std::mutex samplingFinishedMutex;
	std::condition_variable samplingFinishedCV;

	size_t preLastStackSize = 0;
	size_t lastStackSize = 0;

	// In contrast to bool, 32 bit integer has atomic read and write operations
	int32_t stopped = true;
	int32_t doSampling = false;
	int32_t samplingStarted = false;
	int32_t samplingDone = false;

	METHOD_ID baseMethodId;
	JAVA_LONG startTime;

	void timerMethod();

	void doSamplingNow();
	void triggerSampling();

public:
	StackSizeBasedTrigger(ThreadID threadId, JAVA_LONG maxSamplingInterval);
	~StackSizeBasedTrigger();

	void start(std::shared_ptr<StackTraceSampler> sampler, METHOD_ID baseMethodId = 0, JAVA_LONG startTime = -1);
	void stop(JAVA_LONG stopTime = -1);

	std::shared_ptr<StackTraceSampler> getSampler();

	void stackSizeWillChange(size_t newSize);
	void stackSizeChanged(size_t newSize);
};

