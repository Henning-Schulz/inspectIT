#pragma once

#include "basehdr.h"
#include "StackTraceSample.h"
#include "ShadowStackListener.h"
#include "Logger.h"

#include <vector>
#include <utility>
#include <mutex>
#include <memory>
#include <chrono>

class ShadowStack
{
private:
	ThreadID threadId;
	std::vector<std::pair<METHOD_ID, bool>> stack;
	std::mutex stackMutex;

	std::vector<std::shared_ptr<ShadowStackListener>> listeners;

	Logger logger = loggerFactory.createLogger("ShadowStack");

	void notifyListenersBeforeChange(size_t newSize);
	void notifyListenersAfterChange(size_t newSize);

public:
	ShadowStack(ThreadID threadId);
	~ShadowStack();

	void pushMethod(METHOD_ID methodId);
	void popMethod();

	std::shared_ptr<StackTraceSample> doSnapshot();

	void addListener(std::shared_ptr<ShadowStackListener> listener);
	void removeListeners();
};