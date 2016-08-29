#include "ShadowStackProvider.h"



ShadowStackProvider::ShadowStackProvider()
{
}


ShadowStackProvider::~ShadowStackProvider()
{
	logger.debug("Destructor");
}

void ShadowStackProvider::init() {
	hook = std::make_shared<ShadowStackHook>(shared_from_this());
	hook->setPriority(NORMAL);
}

bool ShadowStackProvider::hasHook()
{
	return true;
}

std::shared_ptr<MethodHook> ShadowStackProvider::getHook()
{
	return hook;
}

DWORD ShadowStackProvider::getSpecialMonitorFlags()
{
	return 0;
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

		if (it != end) {
			stack = it->second;
		}
	}

	if (it == end) {
		{
			std::unique_lock<std::shared_mutex> uniqueLock(stacksMutex);

			it = stacksPerThread.find(threadId);
			if (it == stacksPerThread.end()) {
				stack = std::make_shared<ShadowStack>(threadId);
				stacksPerThread.insert(std::make_pair(threadId, stack));
			}
		}
	}

	stack->pushMethod(methodId);
}

void ShadowStackProvider::popMethod(ThreadID threadId)
{
	std::shared_ptr<ShadowStack> stack;
	std::map<ThreadID, std::shared_ptr<ShadowStack>>::iterator  it;
	std::map<ThreadID, std::shared_ptr<ShadowStack>>::iterator end;

	{
		std::shared_lock<std::shared_mutex> sharedLock(stacksMutex);
		it = stacksPerThread.find(threadId);
		end = stacksPerThread.end();

		if (it != end) {
			stack = it->second;
		}
	}

	if (it == end) {
		logger.warn("No stack for thread %i present. Cannot pop method.", threadId);
	} else {
		stack->popMethod();
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

void ShadowStackProvider::addShadowStackListener(ThreadID threadId, std::shared_ptr<ShadowStackListener> listener)
{
	std::shared_ptr<ShadowStack> stack;
	std::map<ThreadID, std::shared_ptr<ShadowStack>>::iterator  it;
	std::map<ThreadID, std::shared_ptr<ShadowStack>>::iterator end;

	{
		std::shared_lock<std::shared_mutex> sharedLock(stacksMutex);
		it = stacksPerThread.find(threadId);
		end = stacksPerThread.end();

		if (it != end) {
			stack = it->second;
		}
	}

	if (it == end) {
		{
			std::unique_lock<std::shared_mutex> uniqueLock(stacksMutex);

			it = stacksPerThread.find(threadId);
			if (it == stacksPerThread.end()) {
				stack = std::make_shared<ShadowStack>(threadId);
				stacksPerThread.insert(std::make_pair(threadId, stack));
			}
		}
	}

	stack->addListener(listener);
}
