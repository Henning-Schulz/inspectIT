#pragma once

#include "SensorData.h"

#include <memory>

class BufferStrategy
{
public:
	BufferStrategy() {}
	virtual ~BufferStrategy() {}

	virtual void addMeasurements(std::vector<std::shared_ptr<SensorData>> data) = 0;
	virtual bool hasNext() = 0;
	virtual std::vector<std::shared_ptr<SensorData>> next() = 0;
};