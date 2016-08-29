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
	notifyListenersBeforeChange(stack.size() + 1);
	{
		std::unique_lock<std::mutex> lock(stackMutex);
		stack.push_back(std::make_pair(methodId, true));
	}
	notifyListenersAfterChange(stack.size());
}

void ShadowStack::popMethod()
{
	notifyListenersBeforeChange(stack.size() - 1);
	{
		std::unique_lock<std::mutex> lock(stackMutex);
		stack.pop_back();
	}
	notifyListenersAfterChange(stack.size());
}

std::shared_ptr<StackTraceSample> ShadowStack::doSnapshot()
{
	JAVA_LONG timestamp = 0;
	std::vector<JAVA_LONG> trace;
	JAVA_INT offset;

	{
		std::unique_lock<std::mutex> lock(stackMutex);

		timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		offset = stack.size();
		for (auto it = stack.rbegin(); it != stack.rend() && it->second; it++) {
			trace.push_back(it->first);
			it->second = false;
			offset--;
		}
	}

	std::shared_ptr<StackTraceSample> sample = std::make_shared<StackTraceSample>(trace, offset, timestamp);
	return sample;
}

void ShadowStack::addListener(std::shared_ptr<ShadowStackListener> listener)
{
	listeners.push_back(listener);
}

void ShadowStack::removeListeners()
{
	listeners.clear();
}

void ShadowStack::notifyListenersBeforeChange(size_t newSize)
{
	for (auto it = listeners.begin(); it != listeners.end(); it++) {
		(*it)->stackSizeWillChange(newSize);
	}
}

void ShadowStack::notifyListenersAfterChange(size_t newSize)
{
	for (auto it = listeners.begin(); it != listeners.end(); it++) {
		(*it)->stackSizeChanged(newSize);
	}
}
