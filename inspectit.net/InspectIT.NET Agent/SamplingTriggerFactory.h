#pragma once

#include "StackTraceSensorConfig.h"
#include "SamplingTrigger.h"
#include "ShadowStackProvider.h"

#include <memory>

class SamplingTriggerFactory
{
private:
	TriggerType triggerType;
	JAVA_LONG samplingInterval;

	std::shared_ptr<ShadowStackProvider> shadowStackProvider;

public:
	SamplingTriggerFactory(TriggerType triggerType, JAVA_LONG samplingInterval = -1);
	~SamplingTriggerFactory();

	std::shared_ptr<SamplingTrigger> createSamplingTrigger(ThreadID threadId);

	void setShadowStackProvider(std::shared_ptr<ShadowStackProvider> shadowStackProvider);
};

