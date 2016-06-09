#include "ShadowStack.h"


ShadowStack::ShadowStack(ThreadID threadId)
{
	this->threadId = threadId;
}


ShadowStack::~ShadowStack()
{
}

void ShadowStack::pushMethod(METHOD_ID methodId)
{
	std::unique_lock<std::mutex> lock(stackMutex);
	logger.debug("push_back(%i)", methodId);
	stack.push_back(std::make_pair(methodId, true));
}

void ShadowStack::popMethod()
{
	std::unique_lock<std::mutex> lock(stackMutex);
	logger.debug("pop_back(%i)", stack.back().first);
	stack.pop_back();
	if (stack.empty()) {
		logger.debug("Stack is empty");
	}
}

std::shared_ptr<StackTraceSample> ShadowStack::doSnapshot()
{
	std::vector<JAVA_LONG> trace;
	JAVA_INT offset;

	{
		std::unique_lock<std::mutex> lock(stackMutex);

		offset = stack.size();
		for (auto it = stack.rbegin(); it != stack.rend() && it->second; it++) {
			trace.push_back(it->first);
			it->second = false;
			offset--;
		}
	}

	return std::make_shared<StackTraceSample>(trace, offset);
}