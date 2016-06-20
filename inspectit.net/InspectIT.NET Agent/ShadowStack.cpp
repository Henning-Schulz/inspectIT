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
	stack.push_back(std::make_pair(methodId, true));
}

void ShadowStack::popMethod()
{
	std::unique_lock<std::mutex> lock(stackMutex);
	stack.pop_back();
}

std::shared_ptr<StackTraceSample> ShadowStack::doSnapshot()
{
	JAVA_LONG timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
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

	return std::make_shared<StackTraceSample>(trace, offset, timestamp);
}