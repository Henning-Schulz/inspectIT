#include "SamplingTriggerFactory.h"

#include "TimedTrigger.h"
#include "RandomizedTimedTrigger.h"
#include "StackSizeBasedTrigger.h"



SamplingTriggerFactory::SamplingTriggerFactory(TriggerType triggerType, JAVA_LONG samplingInterval)
{
	this->triggerType = triggerType;
	this->samplingInterval = samplingInterval;
}


SamplingTriggerFactory::~SamplingTriggerFactory()
{
}

std::shared_ptr<SamplingTrigger> SamplingTriggerFactory::createSamplingTrigger(ThreadID threadId)
{
	std::shared_ptr<SamplingTrigger> trigger;

	switch (triggerType) {
	case TIMER:
		if (samplingInterval <= 0) {
			// sampling interval must be positive
			trigger = std::shared_ptr<SamplingTrigger>();
		}
		else {
			trigger = std::make_shared<TimedTrigger>(threadId, samplingInterval);
		}
		break;
	case RANDOMIZED_TIMER:
		if (samplingInterval <= 0) {
			// sampling interval must be positive
			trigger = std::shared_ptr<RandomizedTimedTrigger>();
		}
		else {
			trigger = std::make_shared<RandomizedTimedTrigger>(threadId, samplingInterval);
		}
		break;
	case STACK_SIZE_BASED:
		if (samplingInterval <= 0 || !shadowStackProvider) {
			// sampling interval must be positive and
			// the shadow stack provider must be specified
			trigger = std::shared_ptr<StackSizeBasedTrigger>();
		}
		else {
			std::shared_ptr<StackSizeBasedTrigger> stbTrigger = std::make_shared<StackSizeBasedTrigger>(threadId, samplingInterval);
			shadowStackProvider->addShadowStackListener(threadId, stbTrigger);
			trigger = stbTrigger;
		}
		break;
	default:
		// Not yet implemented
		trigger = std::shared_ptr<SamplingTrigger>();
		break;
	}

	return trigger;
}

void SamplingTriggerFactory::setShadowStackProvider(std::shared_ptr<ShadowStackProvider> shadowStackProvider)
{
	this->shadowStackProvider = shadowStackProvider;
}