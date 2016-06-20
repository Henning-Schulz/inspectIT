#include "StackTraceSamplingSensor.h"

#include "TimedTrigger.h"
#include "ShadowStackProvider.h"
#include "StackTraceSensorConfig.h"



StackTraceSamplingSensor::StackTraceSamplingSensor()
{
}


StackTraceSamplingSensor::~StackTraceSamplingSensor()
{
}

void StackTraceSamplingSensor::init(std::shared_ptr<MethodSensorConfig> config, JAVA_LONG sensorTypeId, JAVA_LONG platformId, ICorProfilerInfo *profilerInfo)
{
	if (wcscmp(config->getClassName(), StackTraceSensorConfig::CLASS_NAME) != 0) {
		logger.warn("Passed illegal sensor config %ls. Using TimedTrigger with sampling interval 200 ms and ShadowStackProvider.", config->getClassName());
		trigger = std::make_shared<TimedTrigger>(200);
		provider = std::make_shared<ShadowStackProvider>();
	}
	else {
		std::shared_ptr<StackTraceSensorConfig> stsc = std::static_pointer_cast<StackTraceSensorConfig>(config);

		switch (stsc->getTriggerType()) {
		case TIMER:
			trigger = std::make_shared<TimedTrigger>(stsc->getSamplingInterval());
			break;
		case RANDOMIZED_TIMER:
		case STACK_SIZE_BASED:
		default:
			logger.warn("Unknown trigger! Using TimeTrigger with sampling interval 200 ms.");
			trigger = std::make_shared<TimedTrigger>(200);
			break;
		}

		switch (stsc->getProviderType()) {
		case SHADOW_STACK:
			provider = std::make_shared<ShadowStackProvider>();
			break;
		case SIMPLE:
		default:
			logger.warn("Unknown provider! Using ShadowStackProvider.");
			provider = std::make_shared<ShadowStackProvider>();
			break;
		}
	}

	sampler = std::make_shared<StackTraceSampler>(provider);
	
	if (provider->hasHook()) {
		provider->getHook()->setPlatformId(platformId);
		provider->getHook()->setSensorTypeId(sensorTypeId);
		provider->getHook()->init(profilerInfo);
	}

	sampler->setPlatformId(platformId);
	sampler->setSensorTypeId(sensorTypeId);

	trigger->start(sampler);

	logger.debug("StackTraceSamlplerSensor initialized.");
}

void StackTraceSamplingSensor::notifyShutdown()
{
	if (hasHook()) {
		getHook()->notifyShutdown();
	}

	trigger->stop();
}

bool StackTraceSamplingSensor::hasHook()
{
	return provider->hasHook();
}

std::shared_ptr<MethodHook> StackTraceSamplingSensor::getHook()
{
	return provider->getHook();
}