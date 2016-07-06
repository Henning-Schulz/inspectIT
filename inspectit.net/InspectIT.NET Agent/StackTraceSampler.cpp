#include "StackTraceSampler.h"

const std::string PREFIX = "sampler";



StackTraceSampler::StackTraceSampler(std::shared_ptr<StackTraceProvider> provider)
{
	this->provider = provider;
}


StackTraceSampler::~StackTraceSampler()
{
	logger.debug("Deconstructor");
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
	logger.debug("Retrieved %i traces", allTraces.size());
	for (auto it = allTraces.begin(); it != allTraces.end(); it++) {
		storeTrace(it->first, it->second, forceNewStorage);
	}
}

void StackTraceSampler::notifyShutdown()
{
	logger.debug("Finishing storages...");

	for (auto ptid = allThreads.begin(); ptid != allThreads.end(); ptid++) {
		std::shared_ptr<MeasurementStorage> uncastedStorage = dataSendingService->getMeasurementStorage(sensorTypeId, *ptid, PREFIX);
		std::shared_ptr<StackTraceStorage> storage;

		if (uncastedStorage != nullptr) {
			storage = std::static_pointer_cast<StackTraceStorage>(uncastedStorage);
			storage->finish();
			logger.debug("Storage for thread %i finished.", *ptid);
		} else {
			logger.debug("No storage for thread %i existing.", *ptid);
		}
	}

	logger.debug("All storages finished.");
}

void StackTraceSampler::storeTrace(ThreadID threadId, std::shared_ptr<StackTraceSample> trace, bool forceNewStorage)
{
	logger.debug("Storing trace for thread %i...", threadId);

	allThreads.insert(threadId);

	std::shared_ptr<MeasurementStorage> uncastedStorage = dataSendingService->getMeasurementStorage(sensorTypeId, threadId, PREFIX);
	std::shared_ptr<StackTraceStorage> storage;
	bool isNew = false;

	if (uncastedStorage == nullptr) {
		logger.debug("No storage for thread %i existed. Creating new one.", threadId);
		storage = std::make_shared<StackTraceStorage>(platformId, sensorTypeId, threadId);
		dataSendingService->addMeasurementStorage(storage, PREFIX);
		isNew = true;
	}
	else {
		logger.debug("Using existing storage for thread %i", threadId);
		storage = std::static_pointer_cast<StackTraceStorage>(uncastedStorage);
	}

	if (!isNew) {
		JAVA_INT lastStoredOffset = storage->getTraces().back()->getOffset();
		size_t lastStoredSize = storage->getTraces().back()->getTrace().size();

		// new storage is forced OR
		// passed trace is not empty AND has offset 0 AND
		//     last stored trace has no explicitly stored methods OR
		//     passed trace's highest method differs from the storage's

		if (forceNewStorage || (trace->getTrace().size() > 0 && trace->getOffset() == 0 && (lastStoredSize == 0 || storage->getHighestMethod() != trace->getHighestMethod()))) {
			logger.debug("Creating new storage. (forceNewStorage = %i, trace->getOffset() = %ld, trace->getHighestMethod() = %lld, storage->getHighestMethod() = %lld)", forceNewStorage, trace->getOffset(), trace->getHighestMethod(), storage->getHighestMethod());

			// Finish the last storage and create new one
			storage->finish();
			storage = std::make_shared<StackTraceStorage>(platformId, sensorTypeId, threadId);
			dataSendingService->addMeasurementStorage(storage, PREFIX);
		}
	}

	storage->addTrace(trace);
	logger.debug("Trace for thread %i added.", threadId);
}