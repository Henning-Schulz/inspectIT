#include "StackSizeBasedTrigger.h"



StackSizeBasedTrigger::StackSizeBasedTrigger(ThreadID threadId, JAVA_LONG maxSamplingInterval)
{
	this->threadId = threadId;
	this->maxSamplingInterval = std::chrono::milliseconds(maxSamplingInterval);
	this->retryInterval = std::chrono::milliseconds(1);
	this->maxNumberOfRetries = maxSamplingInterval / 10;
	if (this->maxNumberOfRetries < 1) {
		this->maxNumberOfRetries = 1;
	}
}


StackSizeBasedTrigger::~StackSizeBasedTrigger()
{
}

void StackSizeBasedTrigger::start(std::shared_ptr<StackTraceSampler> sampler, METHOD_ID baseMethodId, JAVA_LONG startTime)
{
	this->preLastStackSize = 0;
	this->lastStackSize = 0;
	this->baseMethodId = baseMethodId;
	this->startTime = startTime;
	this->sampler = sampler;
	StackSizeBasedTrigger *that = this;
	stopped = false;
	doSampling = false;
	this->timerThread = std::thread([that]() { that->timerMethod(); });
}

void StackSizeBasedTrigger::stop(JAVA_LONG stopTime)
{
	stopped = true;

	{
		std::unique_lock<std::mutex> lock(timeoutMutex);
		timeoutCV.notify_all();
	}

	timerThread.join();
	sampler->finalizeThread(threadId, baseMethodId, startTime, stopTime);
}

void StackSizeBasedTrigger::stackSizeWillChange(size_t newSize)
{
	if (!stopped) {
		if (newSize < lastStackSize && preLastStackSize < lastStackSize) {
			doSamplingNow();
		}

		preLastStackSize = lastStackSize;
		lastStackSize = newSize;
	}
}

void StackSizeBasedTrigger::stackSizeChanged(size_t newSize)
{
	// do nothing
}

std::shared_ptr<StackTraceSampler> StackSizeBasedTrigger::getSampler()
{
	return this->sampler;
}

void StackSizeBasedTrigger::timerMethod()
{
	while (!stopped) {
		std::cv_status status = std::cv_status::no_timeout;
		auto sleepStopTime = std::chrono::system_clock::now() + maxSamplingInterval;
		while (!stopped && !doSampling && status != std::cv_status::timeout) {
			std::unique_lock<std::mutex> lock(timeoutMutex);
			status = timeoutCV.wait_until(lock, sleepStopTime);
		}

		if (stopped) {
			break;
		}

		bool notifySamplingDone = false;

		if (doSampling)
		{
			notifySamplingDone = true;
			std::unique_lock<std::mutex> startSamplingLock(startSamplingMutex);
			samplingStarted = true;
			startSamplingCV.notify_all();
		}

		triggerSampling();

		if (notifySamplingDone)
		{
			std::unique_lock<std::mutex> samplingDoneLock(samplingDoneMutex);
			samplingDone = true;
			samplingDoneCV.notify_all();
		}
	}
}

void StackSizeBasedTrigger::doSamplingNow()
{
	samplingStarted = false;
	samplingDone = false;

	std::unique_lock<std::mutex> startSamplingLock(startSamplingMutex);
	std::unique_lock<std::mutex> samplingDoneLock(samplingDoneMutex);

	doSampling = true;

	{
		// Notify the timer thread to stop sleeping
		std::unique_lock<std::mutex> lock(timeoutMutex);
		timeoutCV.notify_all();
	}

	// Wait for new sampling
	while (!samplingStarted) {
		startSamplingCV.wait(startSamplingLock);
	}
	startSamplingLock.unlock();

	doSampling = false;

	// Wait until sampling done
	while (!samplingDone) {
		samplingDoneCV.wait(samplingDoneLock);
	}
}

void StackSizeBasedTrigger::triggerSampling()
{
	bool succeeded = false;
	int retryCount = -1;
	while (!stopped && !succeeded && retryCount < maxNumberOfRetries) {
		retryCount++;

		if (retryCount > 0) {
			std::this_thread::sleep_for(retryInterval);
		}

		succeeded = sampler->doSampling(threadId, false);
	}

	if (retryCount > 0) {
		logger.debug("Retried %i times", retryCount);
		if (!succeeded && !stopped) {
			logger.warn("Could not retrieve stack trace within %i tries!", retryCount + 1);
		}
	}
}