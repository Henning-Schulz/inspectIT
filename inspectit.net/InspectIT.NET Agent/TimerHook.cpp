#include "TimerHook.h"

#include "MeasurementStorage.h"
#include "DataSendingService.h"
#include "TimerData.h"

using namespace inspectit::sending;
using namespace inspectit::sensor::timer;
using namespace inspectit::storage;

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

thread_local std::map<JAVA_LONG, std::shared_ptr<inspectit::sending::SensorDataWrapper>> storedSensorData;

void TimerHook::beforeBody(METHOD_ID methodID)
{
	ThreadID threadId = 0;
	profilerInfo->GetCurrentThreadID(&threadId);

	auto startNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

	std::shared_ptr<SensorDataWrapper> dataWrapper = getDataSendingService()->getDataWrapper();
	if (!dataWrapper) {
		logger.debug("Did not get data wrapper for TimerHook and method %i", methodID);
		return;
	}

	std::shared_ptr<TimerData> timerData = std::make_shared<TimerData>(getPlatformId(), getSensorTypeId(), methodID, threadId, startNanos.count());
	timerData->setStartTime(startNanos.count());
	dataWrapper->setData(timerData);

	storedSensorData.emplace(methodID, dataWrapper);
}

void TimerHook::afterBody(METHOD_ID methodID)
{
	auto endNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

	ThreadID threadId = 0;
	profilerInfo->GetCurrentThreadID(&threadId);

	auto it = storedSensorData.find(methodID);

	if (it == storedSensorData.end()) {
		logger.debug("There is no data wrapper for TimerHook and method %i", methodID);
		return;
	}

	std::shared_ptr<SensorDataWrapper> dataWrapper = it->second;
	storedSensorData.erase(it);

	std::shared_ptr<TimerData> timerData = std::static_pointer_cast<TimerData>(dataWrapper->getData());
	timerData->setEndTime(endNanos.count());

	getDataSendingService()->publishDataWrapper(dataWrapper);
}