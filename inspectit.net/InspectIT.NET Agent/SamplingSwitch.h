#pragma once
#include "SamplingTrigger.h"
#include "MethodHook.h"
#include "ThreadHook.h"
#include "SamplingTriggerFactory.h"
#include "Logger.h"

#include <map>
#include <mutex>

class SamplingSwitch : public SamplingTrigger, public MethodHook, public ThreadHook
{
private:
	ICorProfilerInfo *profilerInfo;
	std::shared_ptr<StackTraceSampler> sampler;

	std::shared_ptr<SamplingTriggerFactory> triggerFactory;
	std::map<ThreadID, std::shared_ptr<SamplingTrigger>> triggers;
	std::shared_mutex mTriggers;

	bool active = false;

	Logger logger = loggerFactory.createLogger("SamplingSwitch");

public:
	SamplingSwitch(std::shared_ptr<StackTraceSensorConfig> config);
	~SamplingSwitch();

	void start(std::shared_ptr<StackTraceSampler> sampler, METHOD_ID baseMethodId = 0, JAVA_LONG startTime = -1);
	void stop(JAVA_LONG stopTime = -1);
	std::shared_ptr<StackTraceSampler> getSampler();

	void init(ICorProfilerInfo *profilerInfo);
	void notifyShutdown();

	void beforeBody(METHOD_ID methodID);
	void afterBody(METHOD_ID methodID);

	void threadCreated(ThreadID threadId);
	void threadDestroyed(ThreadID threadId);

	void setShadowStackProvider(std::shared_ptr<ShadowStackProvider> shadowStackProvider);
};

