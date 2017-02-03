#include "SimpleBufferStrategy.h"



SimpleBufferStrategy::SimpleBufferStrategy()
{
}


SimpleBufferStrategy::~SimpleBufferStrategy()
{
}

void SimpleBufferStrategy::addMeasurements(std::vector<std::shared_ptr<MethodSensorData>> data)
{
	// Creates a unique lock on the mutex
	std::unique_lock<std::mutex> lock(measurementsMutex);

	this->newMeasurements = true;
	this->measurements = data;
	// The mutex is automaticly freed, since the lock reference is only valid
	// in the scope of this method
}

bool SimpleBufferStrategy::hasNext()
{
	return newMeasurements;
}

std::vector<std::shared_ptr<MethodSensorData>> SimpleBufferStrategy::next()
{
	std::unique_lock<std::mutex> lock(measurementsMutex);

	if (newMeasurements) {
		newMeasurements = false;
		return measurements;
	}
	else {
		logger.error("No new measurements available!");
		return std::vector<std::shared_ptr<MethodSensorData>>();
	}
}