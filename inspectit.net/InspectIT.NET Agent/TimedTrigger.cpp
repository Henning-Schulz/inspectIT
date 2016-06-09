#include "TimedTrigger.h"



TimedTrigger::TimedTrigger()
{
}


TimedTrigger::~TimedTrigger()
{
}

void TimedTrigger::start(std::shared_ptr<StackTraceSampler> sampler)
{
	this->sampler = sampler;
	TimedTrigger *that = this;
	this->worker = std::thread([that]() { that->workerMethod(); });
}

void TimedTrigger::stop()
{
	stopped = true;
	worker.join();
}

void TimedTrigger::workerMethod()
{
	while (!stopped) {
		sampler->doSamplingOfAllThreads(false);
		std::this_thread::sleep_for(std::chrono::milliseconds(SAMPLING_INTERVAL));
	}

	sampler->doSamplingOfAllThreads(true);
}