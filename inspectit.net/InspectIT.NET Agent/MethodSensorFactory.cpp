#include "MethodSensorFactory.h"

#include "TimerSensor.h"


void MethodSensorFactory::setupKnownSensors()
{
	addKnownSensor(std::make_shared<TimerSensor>());
}

void MethodSensorFactory::addKnownSensor(std::shared_ptr<MethodSensor> sensor)
{
	knownSensors.emplace(sensor->getName(), sensor);
}

MethodSensorFactory::MethodSensorFactory()
{
}


MethodSensorFactory::~MethodSensorFactory()
{
}

std::shared_ptr<MethodSensor> MethodSensorFactory::createMethodSensor(std::shared_ptr<MethodSensorTypeConfig> sensorTypeConfig, JAVA_LONG platformId, ICorProfilerInfo3 * profilerInfo)
{
	auto it = knownSensors.find(sensorTypeConfig->getName());
	if (it == knownSensors.end()) {
		// TODO: Error!
		return std::shared_ptr<MethodSensor>();
	}
	else {
		auto sensor = it->second;
		sensor->initialize(sensorTypeConfig, platformId, profilerInfo);
		return sensor;
	}
}
