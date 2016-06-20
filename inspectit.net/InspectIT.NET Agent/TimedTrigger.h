#pragma once

#include "SamplingTrigger.h"
#include "Logger.h"

#include <vector>
#include <thread>
#include <chrono>
#include <memory>

class TimedTrigger : public SamplingTrigger
{
private:
	Logger logger = loggerFactory.createLogger("TimedTrigger");

	std::shared_ptr<StackTraceSampler> sampler;
	std::thread worker;
	JAVA_LONG samplingInterval;
	bool stopped = false;

	void workerMethod();

public:
	TimedTrigger(JAVA_LONG samplingInterval);
	~TimedTrigger();

	void start(std::shared_ptr<StackTraceSampler> sampler);
	void stop();
};

