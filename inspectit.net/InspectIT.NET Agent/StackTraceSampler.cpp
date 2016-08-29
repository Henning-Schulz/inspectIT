#include "StackTraceSampler.h"

const std::string PREFIX = "sampler";



StackTraceSampler::StackTraceSampler(std::shared_ptr<StackTraceProvider> provider)
{
	this->provider = provider;
}


StackTraceSampler::~StackTraceSampler()
{
	logger.debug("Destructor");
}

void StackTraceSampler::setSensorTypeId(JAVA_LONG sensorTypeId)
{
	this->sensorTypeId = sensorTypeId;
}

void StackTraceSampler::setPlatformId(JAVA_LONG platformId)
{
	this->platformId = platformId;
}

bool StackTraceSampler::doSampling(ThreadID threadId, bool forceNewStorage)
{
	std::shared_ptr<StackTraceSample> trace = provider->getStackTrace(threadId);

	if (!trace) {
		return false;
	}
	else {
		storeTrace(threadId, trace, forceNewStorage);
		return true;
	}
}

void StackTraceSampler::finalizeThread(ThreadID threadId, METHOD_ID baseMethodId, JAVA_LONG startTime, JAVA_LONG endTime)
{
	std::shared_ptr<MeasurementStorage> uncastedStorage = dataSendingService->getMeasurementStorage(sensorTypeId, threadId, PREFIX);
	std::shared_ptr<StackTraceStorage> storage;

	if (uncastedStorage != nullptr) {
		storage = std::static_pointer_cast<StackTraceStorage>(uncastedStorage);

		if (baseMethodId > 0) {
			storage->setBaseMethodId(baseMethodId);
		}

		if (startTime >= 0) {
			storage->setStartTime(startTime);
		}

		if (endTime >= 0) {
			storage->setEndTime(endTime);
		}

		storage->finish();
		dataSendingService->notifyStorageFinished(storage, PREFIX);
	}
}

void StackTraceSampler::storeTrace(ThreadID threadId, std::shared_ptr<StackTraceSample> trace, bool forceNewStorage)
{
	if (!trace) {
		logger.warn("Passed trace for thread %i was NULL. It will not be stored.", threadId);
		return;
	}

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
		std::shared_ptr<StackTraceSample> storedTrace = storage->getTraces().back();
		JAVA_INT lastStoredOffset = storedTrace->getOffset();
		size_t lastStoredSize = storedTrace->getTrace().size();

		// new storage is forced OR
		// storage is already finished OR
		// passed trace is not empty AND has offset 0 AND
		//     last stored trace has no explicitly stored methods OR
		//     passed trace's highest method differs from the storage's

		if (forceNewStorage || storage->finished() || (trace->getTrace().size() > 0 && trace->getOffset() == 0 && (lastStoredSize == 0 || storage->getHighestMethod() != trace->getHighestMethod()))) {
			if (loggerFactory.getLogLevel() >= LEVEL_DEBUG) {
				if (forceNewStorage) {
					logger.debug("Creating new storage (was forced).");
				}
				else if (storage->finished()) {
					logger.debug("Creating new storage (stored storage is finished).");
				}
				else {
					logger.debug("Creating new storage (new base method).");
					logger.debug("trace->getTrace().size() = %i", trace->getTrace().size());
					logger.debug("trace->getOffset() = %li", trace->getOffset());
					logger.debug("lastStoredSize = %i", lastStoredSize);
					logger.debug("storage->getHighestMethod() = %lli", storage->getHighestMethod());
					logger.debug("trace->getHighestMethod() = %lli", trace->getHighestMethod());
				}
			}

			// Finish the last storage and create new one
			storage->finish();
			dataSendingService->notifyStorageFinished(storage, PREFIX);
			storage = std::make_shared<StackTraceStorage>(platformId, sensorTypeId, threadId);
			dataSendingService->addMeasurementStorage(storage, PREFIX);
		}
	}

	storage->addTrace(trace);
}