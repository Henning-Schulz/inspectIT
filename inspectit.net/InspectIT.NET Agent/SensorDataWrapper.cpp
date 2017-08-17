#include "SensorDataWrapper.h"

using namespace inspectit::sending;

SensorDataWrapper::SensorDataWrapper()
{
}


SensorDataWrapper::~SensorDataWrapper()
{
}

std::shared_ptr<inspectit::data::SensorData> inspectit::sending::SensorDataWrapper::getData()
{
	return data;
}

void inspectit::sending::SensorDataWrapper::setData(std::shared_ptr<inspectit::data::SensorData> data)
{
	this->data = data;
}

void inspectit::sending::SensorDataWrapper::setSlot(size_t slot)
{
	this->slot = slot;
}

size_t inspectit::sending::SensorDataWrapper::getSlot()
{
	return slot;
}

void inspectit::sending::SensorDataWrapper::finish()
{
	this->finished = true;
}

void inspectit::sending::SensorDataWrapper::reset()
{
	this->finished = false;
}

int inspectit::sending::SensorDataWrapper::isFinished()
{
	return finished;
}
