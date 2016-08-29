#include "TimerStorage.h"

#include "TimerData.h"



TimerStorage::TimerStorage(JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, ThreadID threadId, std::chrono::duration<long long, std::nano> startNanos)
	: MeasurementStorage(platformId, methodSensorId, methodId, std::chrono::duration_cast<std::chrono::milliseconds>(startNanos).count())
{
	this->startNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(startNanos).count();
	this->threadId = threadId;
}


TimerStorage::~TimerStorage()
{
}

void TimerStorage::setEndTime(std::chrono::duration<long long, std::nano> endNanos)
{
	this->endNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(endNanos).count();
}

bool TimerStorage::finished()
{
	return endNanos > 0;
}

std::shared_ptr<MethodSensorData> TimerStorage::finalizeData()
{
	std::shared_ptr<TimerData> timerData = std::make_shared<TimerData>(getPlatformId(), getMethodSensorId(), getMethodId(), threadId, getTimestamp(), startNanos, endNanos);
	return timerData;
}
