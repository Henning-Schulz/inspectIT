#include "ShadowStackProvider.h"



ShadowStackProvider::ShadowStackProvider()
{
	std::shared_ptr<ShadowStackHolder> thisHolder(this);
	hook = std::make_shared<ShadowStackHook>(thisHolder);
}


ShadowStackProvider::~ShadowStackProvider()
{
}

bool ShadowStackProvider::hasHook()
{
	return true;
}

std::shared_ptr<MethodHook> ShadowStackProvider::getHook()
{
	return hook;
}

void ShadowStackProvider::pushMethod(ThreadID threadId, METHOD_ID methodId)
{
	std::shared_ptr<ShadowStack> stack;
	std::map<ThreadID, std::shared_ptr<ShadowStack>>::iterator  it;
	std::map<ThreadID, std::shared_ptr<ShadowStack>>::iterator end;

	{
		std::shared_lock<std::shared_mutex> sharedLock(stacksMutex);
		it = stacksPerThread.find(threadId);
		end = stacksPerThread.end();
	}

	if (it == end) {
		{
			std::unique_lock<std::shared_mutex> uniqueLock(stacksMutex);

			it = stacksPerThread.find(threadId);
			if (it == stacksPerThread.end()) {
				stack = std::make_shared<ShadowStack>(threadId);
				stacksPerThread.insert(std::make_pair(threadId, stack));
				logger.debug("Created new shadow stack for thread %i", threadId);
			}
		}
	} else {
		stack = it->second;
	}

	stack->pushMethod(methodId);
}

void ShadowStackProvider::popMethod(ThreadID threadId)
{
	std::map<ThreadID, std::shared_ptr<ShadowStack>>::iterator  it;
	std::map<ThreadID, std::shared_ptr<ShadowStack>>::iterator end;

	{
		std::shared_lock<std::shared_mutex> sharedLock(stacksMutex);
		it = stacksPerThread.find(threadId);
		end = stacksPerThread.end();
	}

	if (it == end) {
		logger.warn("No stack for thread %i present. Cannot pop method.", threadId);
	} else {
		it->second->popMethod();
	}
}

std::shared_ptr<StackTraceSample> ShadowStackProvider::getStackTrace(ThreadID threadId)
{
	std::map<ThreadID, std::shared_ptr<ShadowStack>>::iterator  it;
	std::map<ThreadID, std::shared_ptr<ShadowStack>>::iterator end;

	{
		std::shared_lock<std::shared_mutex> sharedLock(stacksMutex);
		it = stacksPerThread.find(threadId);
		end = stacksPerThread.end();
	}

	if (it == end) {
		logger.warn("No stack for thread %i present. Returning empty stack trace.", threadId);
		return std::make_shared<StackTraceSample>(std::vector<JAVA_LONG>(), 0);
	}
	else {
		return it->second->doSnapshot();
	}
}

std::map<ThreadID, std::shared_ptr<StackTraceSample>> ShadowStackProvider::getAllStackTraces()
{
	std::shared_lock<std::shared_mutex> lock(stacksMutex);

	std::map<ThreadID, std::shared_ptr<StackTraceSample>> allTraces;

	for (auto it = stacksPerThread.begin(); it != stacksPerThread.end(); it++) {
		allTraces.insert(std::make_pair(it->first, it->second->doSnapshot()));
	}

	return allTraces;
}