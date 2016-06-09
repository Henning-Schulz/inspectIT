#pragma once

#include "StackTraceSampler.h"

class SamplingTrigger
{
public:
	SamplingTrigger() {}
	virtual ~SamplingTrigger() {}

	virtual void start(std::shared_ptr<StackTraceSampler> sampler) = 0;
	virtual void stop() = 0;
};