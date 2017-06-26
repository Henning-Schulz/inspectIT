#pragma once

#include "BufferStrategy.h"
#include "Logger.h"

#include <mutex>

class SimpleBufferStrategy : public BufferStrategy
{
private:
	std::vector<std::shared_ptr<SensorData>> measurements;
	bool newMeasurements = false;
	std::mutex measurementsMutex;

	Logger logger = loggerFactory.createLogger("SimpleBufferStrategy");

public:
	SimpleBufferStrategy();
	~SimpleBufferStrategy();

	void addMeasurements(std::vector<std::shared_ptr<SensorData>> data);
	bool hasNext();
	std::vector<std::shared_ptr<SensorData>> next();
};

