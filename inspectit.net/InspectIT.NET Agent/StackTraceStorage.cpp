#include "StackTraceStorage.h"


/* 
 * The methodId is set to the threadId in order to use it for identification in DataSendingService
 */
StackTraceStorage::StackTraceStorage(JAVA_LONG platformId, JAVA_LONG methodSensorId, ThreadID threadId, std::time_t timestamp)
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
	return std::make_shared<StackTraceData>(traces, getPlatformId(), getMethodSensorId(), getHighestMethod(), getTimestamp(), getThreadId());
}