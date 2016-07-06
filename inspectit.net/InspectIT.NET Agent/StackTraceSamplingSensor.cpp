#include "StackTraceSamplingSensor.h"

#include "TimedTrigger.h"
#include "ShadowStackProvider.h"
#include "StackTraceSensorConfig.h"
#include "NativeStackProvider.h"



StackTraceSamplingSensor::StackTraceSamplingSensor()
{
}


StackTraceSamplingSensor::~StackTraceSamplingSensor()
{
	logger.debug("Deconstructor");
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
		logger.debug("Initializing StackTraceSamplingSensor");

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

		logger.debug("Setting provider...");

		switch (stsc->getProviderType()) {
		case SHADOW_STACK:
			provider = std::make_shared<ShadowStackProvider>();
			break;
		case SIMPLE:
			logger.debug("Using native stack provider...");
			if (static_cast<ICorProfilerInfo2*>(profilerInfo)) {
				logger.debug("Creating provider...");
				provider = std::make_shared<NativeStackProvider>(static_cast<ICorProfilerInfo2*>(profilerInfo));
				break;
			}
			else {
				logger.error("Passed ICorProfilerInfo is not an ICorProfilerInfo2, which is needed for the NativeStackProvider!");
			}
		default:
			logger.warn("Unable to instantiate provider! Using ShadowStackProvider.");
			provider = std::make_shared<ShadowStackProvider>();
			break;
		}

		logger.debug("Provider set.");
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

bool StackTraceSamplingSensor::hasThreadHook()
{
	return provider->hasThreadHook();
}

std::shared_ptr<ThreadHook> StackTraceSamplingSensor::getThreadHook()
{
	return provider->getThreadHook();
}

DWORD StackTraceSamplingSensor::getSpecialMonitorFlags()
{
	return provider->getSpecialMonitorFlags();
}