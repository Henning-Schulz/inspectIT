#pragma once

#include "basehdr.h"
#include "StackTraceSample.h"
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

	Logger logger = loggerFactory.createLogger("ShadowStack");

public:
	ShadowStack(ThreadID threadId);
	~ShadowStack();

	void pushMethod(METHOD_ID methodId);
	void popMethod();

	std::shared_ptr<StackTraceSample> doSnapshot();
};