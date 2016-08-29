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
	logger.debug("Destructor");
}

void StackTraceSamplingSensor::init(std::shared_ptr<MethodSensorConfig> config, JAVA_LONG sensorTypeId, JAVA_LONG platformId, ICorProfilerInfo *profilerInfo)
{
	setSensorTypeId(sensorTypeId);

	if (wcscmp(config->getClassName(), StackTraceSensorConfig::CLASS_NAME) != 0) {
		logger.warn("Passed illegal sensor config %ls. Disabling stack trace sampling.", config->getClassName());
		return;
	}
	else {
		std::shared_ptr<StackTraceSensorConfig> stsc = std::static_pointer_cast<StackTraceSensorConfig>(config);
		logger.debug("Initializing StackTraceSamplingSensor");

		samplingSwitch = std::make_shared<SamplingSwitch>(stsc);
		samplingSwitch->setPlatformId(getPlatformId());
		samplingSwitch->setSensorTypeId(getSensorTypeId());
		samplingSwitch->init(profilerInfo);

		logger.debug("Setting provider...");

		switch (stsc->getProviderType()) {
		case SHADOW:
			provider = std::make_shared<ShadowStackProvider>();
			samplingSwitch->setShadowStackProvider(std::static_pointer_cast<ShadowStackProvider>(provider));
			break;
		case NATIVE:
			
			("Using native stack provider...");
			if (static_cast<ICorProfilerInfo2*>(profilerInfo)) {
				logger.debug("Creating provider...");
				provider = std::make_shared<NativeStackProvider>(static_cast<ICorProfilerInfo2*>(profilerInfo));
				break;
			}
			else {
				logger.error("Passed ICorProfilerInfo is not an ICorProfilerInfo2, which is needed for the NativeStackProvider!");
				// No break; run into default
			}
		default:
			logger.warn("Unable to instantiate provider! Using ShadowStackProvider.");
			provider = std::make_shared<ShadowStackProvider>();
			break;
		}

		logger.debug("Provider set.");
	}

	provider->init();
	sampler = std::make_shared<StackTraceSampler>(provider);
	
	if (provider->hasHook()) {
		provider->getHook()->setPlatformId(platformId);
		provider->getHook()->setSensorTypeId(sensorTypeId);
		provider->getHook()->init(profilerInfo);
	}

	sampler->setPlatformId(platformId);
	sampler->setSensorTypeId(sensorTypeId);

	logger.debug("StackTraceSamlplingSensor initialized.");
}

std::shared_ptr<StackTraceProvider> StackTraceSamplingSensor::getProvider()
{
	return provider;
}

void StackTraceSamplingSensor::notifyStartup()
{
	samplingSwitch->start(sampler);
	logger.debug("StackTraceSampler started.");
}

void StackTraceSamplingSensor::notifyShutdown()
{
	if (hasHook()) {
		getHook()->notifyShutdown();
	}

	samplingSwitch->stop();
}

bool StackTraceSamplingSensor::hasHook()
{
	return provider->hasHook();
}

std::shared_ptr<MethodHook> StackTraceSamplingSensor::getHook()
{
	return provider->getHook();
}

bool StackTraceSamplingSensor::hasBaseMethodHook()
{
	return true;
}

std::shared_ptr<MethodHook> StackTraceSamplingSensor::getBaseMethodHook()
{
	return samplingSwitch;
}

bool StackTraceSamplingSensor::hasThreadHook()
{
	return true;
}

std::shared_ptr<ThreadHook> StackTraceSamplingSensor::getThreadHook()
{
	return samplingSwitch;
}

DWORD StackTraceSamplingSensor::getSpecialMonitorFlags()
{
	return provider->getSpecialMonitorFlags();
}