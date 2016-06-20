#pragma once

#include "basehdr.h"

#include "MethodHook.h"
#include "ShadowStackHolder.h"
#include "StackTraceSampler.h"
#include "SamplingTrigger.h"

#include <memory>

class ShadowStackHook : public MethodHook
{
private:
	ICorProfilerInfo *profilerInfo;
	std::shared_ptr<ShadowStackHolder> stackHolder;

public:
	ShadowStackHook(std::shared_ptr<ShadowStackHolder> stackHolder);
	~ShadowStackHook();

	void init(ICorProfilerInfo *profilerInfo);
	void notifyShutdown();

	void beforeBody(METHOD_ID methodId);
	void afterBody(METHOD_ID methodId);
};

