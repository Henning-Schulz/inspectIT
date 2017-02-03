#include "TimerHook.h"

#include "MeasurementStorage.h"
#include "DataSendingService.h"
#include "TimerStorage.h"

TimerHook::TimerHook()
{
}


TimerHook::~TimerHook()
{
}

void TimerHook::init(ICorProfilerInfo *profilerInfo)
{
	this->profilerInfo = profilerInfo;
}

void TimerHook::notifyShutdown()
{
}

thread_local std::vector<std::shared_ptr<TimerStorage>> storages;

void TimerHook::beforeBody(METHOD_ID methodID)
{
	ThreadID threadId = 0;
	profilerInfo->GetCurrentThreadID(&threadId);

	auto startNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

	std::shared_ptr<MeasurementStorage> uncastedStorage = dataSendingService->getMeasurementStorage(getSensorTypeId(), threadId);
	std::shared_ptr<TimerStorage> storage;

	if (uncastedStorage == nullptr) {
		storage = std::make_shared<TimerStorage>(getPlatformId(), getSensorTypeId(), threadId, startNanos);
	}
	else {
		storage = std::static_pointer_cast<TimerStorage>(uncastedStorage);
	}

	storage->newEntry(methodID, startNanos);

	if (uncastedStorage == nullptr) {
		dataSendingService->addMeasurementStorage(storage);
	}
}

void TimerHook::afterBody(METHOD_ID methodID)
{
	auto endNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

	ThreadID threadId = 0;
	profilerInfo->GetCurrentThreadID(&threadId);

	std::shared_ptr<MeasurementStorage> uncastedStorage = dataSendingService->getMeasurementStorage(getSensorTypeId(), threadId);
	std::shared_ptr<TimerStorage> storage;

	if (uncastedStorage == nullptr) {
		logger.error("No storage for method %i and thread %i existed!", methodID, threadId);
	}
	else {
		storage = std::static_pointer_cast<TimerStorage>(uncastedStorage);
		storage->finishCurrentEntry(endNanos);

		if (storage->finished()) {
			dataSendingService->notifyStorageFinished(storage);
		}
	}
}