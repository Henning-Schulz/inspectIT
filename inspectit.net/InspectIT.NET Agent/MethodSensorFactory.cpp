#include "MethodSensorFactory.h"

#include "TimerSensor.h"

using namespace inspectit::config;
using namespace inspectit::instrumentation;
using namespace inspectit::logger;
using namespace inspectit::sending;
using namespace inspectit::sensor;
using namespace inspectit::sensor::timer;

void MethodSensorFactory::setupKnownSensors()
{
	addKnownSensor(std::make_shared<TimerSensor>());
}

void MethodSensorFactory::addKnownSensor(std::shared_ptr<MethodSensor> sensor)
{
	knownSensors.emplace(sensor->getClassName(), sensor);
}

MethodSensorFactory::MethodSensorFactory()
{
	setupKnownSensors();
}


MethodSensorFactory::~MethodSensorFactory()
{
}

std::shared_ptr<MethodSensor> MethodSensorFactory::createMethodSensor(std::shared_ptr<MethodSensorTypeConfig> sensorTypeConfig, JAVA_LONG platformId, ICorProfilerInfo3 * profilerInfo, std::shared_ptr<DataSendingService> dataSendingService)
{
	auto it = knownSensors.find(sensorTypeConfig->getClassName());
	if (it == knownSensors.end()) {
		logger.warn("Do not know a sensor %ls with class name %lli and id %lli!", sensorTypeConfig->getName().c_str(), sensorTypeConfig->getClassName().c_str(), sensorTypeConfig->getId());
		if (loggerFactory.getLogLevel() >= LEVEL_INFO) {
			logger.info("Known sensors are:");
			for (auto sit = knownSensors.begin(); sit != knownSensors.end(); sit++) {
				logger.info("%ls - %ls", sit->first.c_str(), sit->second->getName().c_str());
			}
		}

		return std::shared_ptr<MethodSensor>();
	}
	else {
		auto sensor = it->second;
		sensor->initialize(sensorTypeConfig, platformId, profilerInfo, dataSendingService);
		return sensor;
	}
}
