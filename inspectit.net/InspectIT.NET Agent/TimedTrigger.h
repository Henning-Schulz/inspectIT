#pragma once

#include "SamplingTrigger.h"

#include <vector>
#include <thread>
#include <chrono>
#include <memory>

#define SAMPLING_INTERVAL 200

class TimedTrigger : public SamplingTrigger
{
private:
	std::shared_ptr<StackTraceSampler> sampler;
	std::thread worker;
	bool stopped = false;

	void workerMethod();

public:
	TimedTrigger();
	~TimedTrigger();

	void start(std::shared_ptr<StackTraceSampler> sampler);
	void stop();
};

