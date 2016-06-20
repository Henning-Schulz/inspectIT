#include "StackTraceSensorConfig.h"

const LPWSTR StackTraceSensorConfig::CLASS_NAME = L"dotNet.DotNetStackTraceSensor";
const LPWSTR StackTraceSensorConfig::SENSOR_NAME = L"Stack Trace Sampling Sensor";

StackTraceSensorConfig::StackTraceSensorConfig()
{
}


StackTraceSensorConfig::~StackTraceSensorConfig()
{
}

LPWSTR StackTraceSensorConfig::getClassName()
{
	return CLASS_NAME;
}

LPWSTR StackTraceSensorConfig::getName()
{
	return SENSOR_NAME;
}

MethodSensorPriority StackTraceSensorConfig::getPriority()
{
	return priority;
}

void StackTraceSensorConfig::setPriority(MethodSensorPriority priority) {
	this->priority = priority;
}

TriggerType StackTraceSensorConfig::getTriggerType()
{
	return triggerType;
}

void StackTraceSensorConfig::setTriggerType(TriggerType triggerType)
{
	this->triggerType = triggerType;
}

ProviderType StackTraceSensorConfig::getProviderType()
{
	return providerType;
}

void StackTraceSensorConfig::setProviderType(ProviderType providerType)
{
	this->providerType = providerType;
}

JAVA_LONG StackTraceSensorConfig::getSamplingInterval()
{
	return samplingInterval;
}

void StackTraceSensorConfig::setSamplingInterval(JAVA_LONG samplingInterval)
{
	this->samplingInterval = samplingInterval;
}