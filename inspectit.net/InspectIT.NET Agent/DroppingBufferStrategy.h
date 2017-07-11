#pragma once
#include "BufferStrategy.h"
class DroppingBufferStrategy :
	public BufferStrategy
{
public:
	DroppingBufferStrategy();
	~DroppingBufferStrategy();

	void addMeasurements(std::vector<std::shared_ptr<SensorData>> data);
	bool hasNext();
	std::vector<std::shared_ptr<SensorData>> next();
};

