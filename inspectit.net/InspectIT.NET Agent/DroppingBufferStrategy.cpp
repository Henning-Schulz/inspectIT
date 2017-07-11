#include "DroppingBufferStrategy.h"



DroppingBufferStrategy::DroppingBufferStrategy()
{
}


DroppingBufferStrategy::~DroppingBufferStrategy()
{
}

void DroppingBufferStrategy::addMeasurements(std::vector<std::shared_ptr<SensorData>> data)
{
	// do nothing
}

bool DroppingBufferStrategy::hasNext()
{
	return false;
}

std::vector<std::shared_ptr<SensorData>> DroppingBufferStrategy::next()
{
	return std::vector<std::shared_ptr<SensorData>>();
}
