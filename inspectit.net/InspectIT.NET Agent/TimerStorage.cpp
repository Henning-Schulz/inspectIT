#include "TimerStorage.h"

#include "TimerData.h"

using namespace inspectit::data;
using namespace inspectit::sensor::timer;

TimerStorage::TimerStorage(JAVA_LONG platformId, JAVA_LONG methodSensorId, ThreadID threadId, std::chrono::duration<long long, std::nano> timestamp)
	: MeasurementStorage(platformId, methodSensorId, threadId, std::chrono::duration_cast<std::chrono::milliseconds>(timestamp).count())
{
	this->threadId = threadId;
}


TimerStorage::~TimerStorage()
{
}

void TimerStorage::newEntry(JAVA_LONG methodId, std::chrono::duration<long long, std::nano> startNanos)
{
	TimerStorageEntry entry(methodId, startNanos);
	entries.push_back(entry);
	started = true;
}

void TimerStorage::finishCurrentEntry(std::chrono::duration<long long, std::nano> endNanos)
{
	TimerStorageEntry entry = entries.back();
	entry.setEndTime(endNanos);
	finishedEntries.push_back(entry);
	entries.pop_back();
}

bool TimerStorage::finished()
{
	return started && entries.size() == 0;
}

std::vector<std::shared_ptr<SensorData>> TimerStorage::finalizeData()
{
	std::vector<std::shared_ptr<SensorData>> result;

	for (auto it = finishedEntries.begin(); it != finishedEntries.end(); it++) {
		std::shared_ptr<TimerData> timerData = std::make_shared<TimerData>(getPlatformId(), getMethodSensorId(), it->getMethodId(), threadId, getTimestamp(), it->getStartNanos(), it->getEndNanos());
		result.push_back(timerData);
	}

	return result;
}
