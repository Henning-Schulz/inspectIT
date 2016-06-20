#include "StackTraceSampler.h"

const std::string PREFIX = "sampler";



StackTraceSampler::StackTraceSampler(std::shared_ptr<StackTraceProvider> provider)
{
	this->provider = provider;
}


StackTraceSampler::~StackTraceSampler()
{
}

void StackTraceSampler::setSensorTypeId(JAVA_LONG sensorTypeId)
{
	this->sensorTypeId = sensorTypeId;
}

void StackTraceSampler::setPlatformId(JAVA_LONG platformId)
{
	this->platformId = platformId;
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

	if (!isNew) {
		JAVA_INT lastStoredOffset = storage->getTraces().back()->getOffset();
		size_t lastStoredSize = storage->getTraces().back()->getTrace().size();

		// new storage is forced OR
		// passed trace is not empty AND
		//     last stored trace is empty OR
		//     passed trace has no offset but the last stored has one OR
		//     passed trace has no offset and the highest method differs from the highest of the storage

		if (forceNewStorage || (trace->getTrace().size() > 0 && (lastStoredSize == 0 || (trace->getOffset() == 0 && (lastStoredOffset > 0 || (trace->getHighestMethod() != storage->getHighestMethod())))))) {
			logger.debug("Creating new storage. (forceNewStorage = %i, trace->getOffset() = %ld, trace->getHighestMethod() = %lld, storage->getHighestMethod() = %lld)", forceNewStorage, trace->getOffset(), trace->getHighestMethod(), storage->getHighestMethod());

			// Finish the last storage and create new one
			storage->finish();
			storage = std::make_shared<StackTraceStorage>(platformId, sensorTypeId, threadId);
			dataSendingService->addMeasurementStorage(storage, PREFIX);
		}
	}

	storage->addTrace(trace);
}