#include "ShadowStackHook.h"

#include "TimedTrigger.h"



ShadowStackHook::ShadowStackHook(std::shared_ptr<ShadowStackHolder> stackHolder)
{
	this->stackHolder = stackHolder;
}


ShadowStackHook::~ShadowStackHook()
{
}

void ShadowStackHook::init(ICorProfilerInfo *profilerInfo)
{
	this->profilerInfo = profilerInfo;
}

void ShadowStackHook::notifyShutdown()
{
}

void ShadowStackHook::beforeBody(METHOD_ID methodId)
{
	ThreadID threadId = 0;
	profilerInfo->GetCurrentThreadID(&threadId);
	stackHolder->pushMethod(threadId, methodId);
}

void ShadowStackHook::afterBody(METHOD_ID methodId)
{
	ThreadID threadId = 0;
	profilerInfo->GetCurrentThreadID(&threadId);
	stackHolder->popMethod(threadId);
}