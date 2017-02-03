#include "TimerStorageEntry.h"



TimerStorageEntry::TimerStorageEntry(METHOD_ID methodId, std::chrono::duration<long long, std::nano> startNanos)
{
	this->methodId = methodId;
	this->startNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(startNanos).count();
}


TimerStorageEntry::~TimerStorageEntry()
{
}

void TimerStorageEntry::setEndTime(std::chrono::duration<long long, std::nano> endNanos)
{
	this->endNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(endNanos).count();
}

METHOD_ID TimerStorageEntry::getMethodId()
{
	return methodId;
}

JAVA_LONG TimerStorageEntry::getStartNanos()
{
	return startNanos;
}

JAVA_LONG TimerStorageEntry::getEndNanos()
{
	return endNanos;
}
