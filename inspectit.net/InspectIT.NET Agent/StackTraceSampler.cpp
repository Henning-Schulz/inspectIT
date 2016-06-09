#include "StackTraceSampler.h"

const std::string PREFIX = "sampler";



StackTraceSampler::StackTraceSampler(std::shared_ptr<StackTraceProvider> provider, JAVA_LONG sensorTypeId, JAVA_LONG platformId)
{
	this->sensorTypeId = sensorTypeId;
	this->platformId = platformId;
	this->provider = provider;
}


StackTraceSampler::~StackTraceSampler()
{
}

void StackTraceSampler::doSampling(ThreadID threadId, bool forceNewStorage)
{
	std::shared_ptr<StackTraceSample> trace = provider->getStackTrace(threadId);
	storeTrace(threadId, trace, forceNewStorage);
}

void StackTraceSampler::doSamplingOfAllThreads(bool forceNewStorage)
{
	std::map<ThreadID, std::shared_ptr<StackTraceSample>> allTraces = provider->getAllStackTraces();
	for (auto it = allTraces.begin(); it != allTraces.end(); it++) {
		storeTrace(it->first, it->second, forceNewStorage);
	}
}

void StackTraceSampler::storeTrace(ThreadID threadId, std::shared_ptr<StackTraceSample> trace, bool forceNewStorage)
{
	logger.debug("Storing trace for thread %i...", threadId);
	std::shared_ptr<MeasurementStorage> uncastedStorage = dataSendingService->getMeasurementStorage(sensorTypeId, threadId, PREFIX);
	std::shared_ptr<StackTraceStorage> storage;
	bool isNew = false;

	if (uncastedStorage == nullptr) {
		storage = std::make_shared<StackTraceStorage>(platformId, sensorTypeId, threadId);
		dataSendingService->addMeasurementStorage(storage, PREFIX);
		isNew = true;
	}
	else {
		storage = std::static_pointer_cast<StackTraceStorage>(uncastedStorage);
	}

	if (!isNew && (forceNewStorage || trace->getOffset() == 0 && trace->getHighestMethod() != storage->getHighestMethod())) {
		logger.debug("Creating new storage. (forceNewStorage = %i, trace->getOffset() = %ld, trace->getHighestMethod() = %lld, storage->getHighestMethod() = %lld)", forceNewStorage, trace->getOffset(), trace->getHighestMethod(), storage->getHighestMethod());

		// Finish the last storage and create new one
		storage->finish();
		storage = std::make_shared<StackTraceStorage>(platformId, sensorTypeId, threadId);
		dataSendingService->addMeasurementStorage(storage, PREFIX);
	}

	storage->addTrace(trace);
	logger.debug("Stored.");
}