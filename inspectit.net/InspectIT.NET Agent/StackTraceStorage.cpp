#include "StackTraceStorage.h"

#include <map>

/* 
 * The methodId is set to the threadId in order to use it for identification in DataSendingService
 */
StackTraceStorage::StackTraceStorage(JAVA_LONG platformId, JAVA_LONG methodSensorId, ThreadID threadId, JAVA_LONG timestamp)
	: MeasurementStorage(platformId, methodSensorId, threadId, timestamp)
{
	this->threadId = threadId;
}


StackTraceStorage::~StackTraceStorage()
{
}

ThreadID StackTraceStorage::getThreadId()
{
	return threadId;
}

METHOD_ID StackTraceStorage::getBaseMethodId() {
	return baseMethodId;
}

void StackTraceStorage::setBaseMethodId(METHOD_ID baseMethodId)
{
	this->baseMethodId = baseMethodId;
}

JAVA_LONG StackTraceStorage::getStartTime()
{
	return startTime;
}

void StackTraceStorage::setStartTime(JAVA_LONG startTime)
{
	this->startTime = startTime;
}

JAVA_LONG StackTraceStorage::getEndTime()
{
	return endTime;
}

void StackTraceStorage::setEndTime(JAVA_LONG endTime)
{
	this->endTime = endTime;
}

std::vector<std::shared_ptr<StackTraceSample>> StackTraceStorage::getTraces()
{
	return traces;
}

JAVA_LONG StackTraceStorage::getHighestMethod()
{
	if (traces.empty()) {
		return -1;
	}
	else {
		return traces.at(0)->getHighestMethod();
	}
}

void StackTraceStorage::addTrace(std::shared_ptr<StackTraceSample> trace)
{
	traces.push_back(trace);
}

void StackTraceStorage::finish()
{
	isFinished = true;
}

bool StackTraceStorage::finished()
{
	return isFinished;
}

std::shared_ptr<MethodSensorData> StackTraceStorage::finalizeData()
{
	std::shared_ptr<StackTraceData> data = std::make_shared<StackTraceData>(traces, getPlatformId(), getMethodSensorId(), getHighestMethod(), getThreadId(), getTimestamp());
	data->setBaseMethodId(baseMethodId);
	data->setStartTime(startTime);
	data->setEndTime(endTime);
	return data;
}