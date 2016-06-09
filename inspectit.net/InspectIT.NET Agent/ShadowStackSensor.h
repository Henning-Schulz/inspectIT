#pragma once

#include "basehdr.h"

#include "MethodSensor.h"
#include "ShadowStackProvider.h"
#include "StackTraceSampler.h"
#include "SamplingTrigger.h"

#include <memory>

class ShadowStackSensor : public MethodSensor
{
private:
	ICorProfilerInfo *profilerInfo;
	std::shared_ptr<ShadowStackProvider> provider;
	std::shared_ptr<SamplingTrigger> trigger;
	std::shared_ptr<StackTraceSampler> sampler;

public:
	ShadowStackSensor();
	~ShadowStackSensor();

	void init(ICorProfilerInfo *profilerInfo);
	void notifyShutdown();

	void beforeBody(METHOD_ID methodId);
	void afterBody(METHOD_ID methodId);
};

