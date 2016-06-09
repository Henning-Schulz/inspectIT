#include "ShadowStackSensor.h"

#include "TimedTrigger.h"



ShadowStackSensor::ShadowStackSensor()
{
}


ShadowStackSensor::~ShadowStackSensor()
{
}

void ShadowStackSensor::init(ICorProfilerInfo *profilerInfo)
{
	this->profilerInfo = profilerInfo;
	provider = std::make_shared<ShadowStackProvider>();
	trigger = std::make_shared<TimedTrigger>();
	sampler = std::make_shared<StackTraceSampler>(provider, getSensorTypeId(), getPlatformId());
	trigger->start(sampler);
}

void ShadowStackSensor::notifyShutdown()
{
	trigger->stop();
}

void ShadowStackSensor::beforeBody(METHOD_ID methodId)
{
	ThreadID threadId = 0;
	profilerInfo->GetCurrentThreadID(&threadId);
	provider->pushMethod(threadId, methodId);
}

void ShadowStackSensor::afterBody(METHOD_ID methodId)
{
	ThreadID threadId = 0;
	profilerInfo->GetCurrentThreadID(&threadId);
	provider->popMethod(threadId);
}