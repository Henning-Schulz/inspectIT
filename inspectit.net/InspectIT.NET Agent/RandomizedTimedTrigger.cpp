#include "RandomizedTimedTrigger.h"



RandomizedTimedTrigger::RandomizedTimedTrigger(ThreadID threadId, JAVA_LONG samplingInterval)
{
	this->threadId = threadId;
	this->samplingInterval = samplingInterval;
	this->retryInterval = std::chrono::milliseconds(1);
	this->maxNumberOfRetries = samplingInterval / 10;
	if (this->maxNumberOfRetries < 1) {
		this->maxNumberOfRetries = 1;
	}

	// Uniform distribution between 1 and 2 * samplingDistribution
	//  -> sampling at t + r, r in (-t, t]
	samplingDistribution = std::uniform_int_distribution<int>(1, 2 * samplingInterval);
}


RandomizedTimedTrigger::~RandomizedTimedTrigger()
{
}

void RandomizedTimedTrigger::start(std::shared_ptr<StackTraceSampler> sampler, METHOD_ID baseMethodId, JAVA_LONG startTime)
{
	this->baseMethodId = baseMethodId;
	this->startTime = startTime;
	stopped = false;
	this->sampler = sampler;
	RandomizedTimedTrigger *that = this;
	this->worker = std::thread([that]() { that->workerMethod(); });
}

void RandomizedTimedTrigger::stop(JAVA_LONG stopTime)
{
	stopped = true;

	{
		std::unique_lock<std::mutex> lock(sleepMutex);
		sleepCV.notify_all();
	}

	worker.join();
	sampler->finalizeThread(threadId, baseMethodId, startTime, stopTime);
}

std::shared_ptr<StackTraceSampler> RandomizedTimedTrigger::getSampler()
{
	return sampler;
}

void RandomizedTimedTrigger::workerMethod()
{
	while (!stopped) {
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
			if (!succeeded) {
				logger.warn("Could not retrieve stack trace within %i tries!", retryCount + 1);
			}
		}

		std::cv_status status = std::cv_status::no_timeout;
		auto sleepStopTime = std::chrono::system_clock::now() + std::chrono::milliseconds(samplingDistribution(randomGenerator));
		while (!stopped && status != std::cv_status::timeout) {
			std::unique_lock<std::mutex> lock(sleepMutex);
			status = sleepCV.wait_until(lock, sleepStopTime);
		}
	}
}