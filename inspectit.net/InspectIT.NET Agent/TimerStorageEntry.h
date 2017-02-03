#pragma once

#include "basehdr.h"

#include <chrono>

class TimerStorageEntry
{
private:
	METHOD_ID methodId;
	JAVA_LONG startNanos = -1;
	JAVA_LONG endNanos = -1;

public:
	TimerStorageEntry(METHOD_ID methodId, std::chrono::duration<long long, std::nano> startNanos);
	~TimerStorageEntry();

	void setEndTime(std::chrono::duration<long long, std::nano> endNanos);

	METHOD_ID getMethodId();
	JAVA_LONG getStartNanos();
	JAVA_LONG getEndNanos();
};

