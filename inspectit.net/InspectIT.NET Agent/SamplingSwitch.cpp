#include "SamplingSwitch.h"



SamplingSwitch::SamplingSwitch(std::shared_ptr<StackTraceSensorConfig> config)
{
	this->triggerFactory = std::make_shared<SamplingTriggerFactory>(config->getTriggerType(), config->getSamplingInterval());
	setPriority(SAMPLING_BASE);
}


SamplingSwitch::~SamplingSwitch()
{
}

void SamplingSwitch::start(std::shared_ptr<StackTraceSampler> sampler, METHOD_ID baseMethodId, JAVA_LONG startTime)
{
	// ignore baseMethodId and startTime
	this->sampler = sampler;
	active = true;
	logger.debug("Sampling switch is active.");
}

void SamplingSwitch::stop(JAVA_LONG stopTime)
{
	// ignore stopTime
	active = false;
}

std::shared_ptr<StackTraceSampler> SamplingSwitch::getSampler()
{
	return sampler;
}

void SamplingSwitch::init(ICorProfilerInfo *profilerInfo)
{
	this->profilerInfo = profilerInfo;
}

void SamplingSwitch::notifyShutdown()
{

}

void SamplingSwitch::beforeBody(METHOD_ID methodID)
{
	if (active) {
		ThreadID threadId;
		profilerInfo->GetCurrentThreadID(&threadId);

		std::shared_ptr<SamplingTrigger> trigger;
		{
			std::shared_lock<std::shared_mutex> lock(mTriggers);
			auto it = triggers.find(threadId);
			// If nothing went wrong, there is a trigger
			trigger = it->second;
		}

		JAVA_LONG timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		trigger->start(sampler, methodID, timestamp);
	}
}

void SamplingSwitch::afterBody(METHOD_ID methodID)
{
	if (active) {
		JAVA_LONG timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		ThreadID threadId;
		profilerInfo->GetCurrentThreadID(&threadId);

		std::shared_ptr<SamplingTrigger> trigger;
		{
			std::shared_lock<std::shared_mutex> lock(mTriggers);
			auto it = triggers.find(threadId);
			// If nothing went wrong, there is a trigger
			trigger = it->second;
		}

		trigger->stop(timestamp);
	}
}

void SamplingSwitch::threadCreated(ThreadID threadId)
{
	std::unique_lock<std::shared_mutex> lock(mTriggers);
	std::shared_ptr<SamplingTrigger> trigger = triggerFactory->createSamplingTrigger(threadId);
	triggers.insert(std::make_pair(threadId, trigger));
	logger.debug("Created new trigger for thread %i.", threadId);
}

void SamplingSwitch::threadDestroyed(ThreadID threadId)
{
	std::unique_lock<std::shared_mutex> lock(mTriggers);
	triggers.erase(threadId);
}

void SamplingSwitch::setShadowStackProvider(std::shared_ptr<ShadowStackProvider> shadowStackProvider)
{
	triggerFactory->setShadowStackProvider(shadowStackProvider);
}