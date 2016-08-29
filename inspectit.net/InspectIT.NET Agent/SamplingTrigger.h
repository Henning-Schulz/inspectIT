#pragma once

#include "StackTraceSampler.h"

class SamplingTrigger
{
public:
	SamplingTrigger() {}
	virtual ~SamplingTrigger() {}

	virtual void start(std::shared_ptr<StackTraceSampler> sampler, METHOD_ID baseMethodId = 0, JAVA_LONG startTime = -1) = 0;
	virtual void stop(JAVA_LONG stopTime = -1) = 0;

	virtual std::shared_ptr<StackTraceSampler> getSampler() = 0;
};