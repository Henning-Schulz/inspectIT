#include "TimedTrigger.h"



TimedTrigger::TimedTrigger(ThreadID threadId, JAVA_LONG samplingInterval)
{
	this->threadId = threadId;
	this->samplingInterval = std::chrono::milliseconds(samplingInterval);
	this->retryInterval = std::chrono::milliseconds(1);
	this->maxNumberOfRetries = samplingInterval / 10;
	if (this->maxNumberOfRetries < 1) {
		this->maxNumberOfRetries = 1;
	}
}


TimedTrigger::~TimedTrigger()
{
}

void TimedTrigger::start(std::shared_ptr<StackTraceSampler> sampler, METHOD_ID baseMethodId, JAVA_LONG startTime)
{
	this->baseMethodId = baseMethodId;
	this->startTime = startTime;
	stopped = false;
	this->sampler = sampler;
	TimedTrigger *that = this;
	this->worker = std::thread([that]() { that->workerMethod(); });
}

void TimedTrigger::stop(JAVA_LONG stopTime)
{
	stopped = true;

	{
		std::unique_lock<std::mutex> lock(sleepMutex);
		sleepCV.notify_all();
	}

	worker.join();
	sampler->finalizeThread(threadId, baseMethodId, startTime, stopTime);
}

void TimedTrigger::workerMethod()
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
		auto sleepStopTime = std::chrono::system_clock::now() + samplingInterval;
		while (!stopped && status != std::cv_status::timeout) {
		    std::unique_lock<std::mutex> lock(sleepMutex);
			status = sleepCV.wait_until(lock, sleepStopTime);
		}
	}
}

std::shared_ptr<StackTraceSampler> TimedTrigger::getSampler()
{
	return sampler;
}