#include "InstrumentationManager.h"

#include "stringutils.h"
#include "MethodSensorFactory.h"

using namespace inspectit::config;
using namespace inspectit::instrumentation;
using namespace inspectit::logger;
using namespace inspectit::sending;
using namespace inspectit::sensor;
using namespace inspectit::types;
using namespace inspectit::utils;

InstrumentationManager::InstrumentationManager(JAVA_LONG platformId, ICorProfilerInfo3 *profilerInfo, std::shared_ptr<DataSendingService> dataSendingService)
{
	this->platformId = platformId;
	this->profilerInfo = profilerInfo;

	this->dataSendingService = dataSendingService;
}


InstrumentationManager::~InstrumentationManager()
{
}

void InstrumentationManager::addSensorTypeConfigs(std::vector<std::shared_ptr<MethodSensorTypeConfig>> configs)
{
	for (auto it = configs.begin(); it != configs.end(); it++) {
		auto conf = *it;
		sensorTypeConfigs.emplace(conf->getId(), conf);
	}
}

void InstrumentationManager::setExcludeClassesPatterns(std::vector<std::wstring> excludeClassesPatterns)
{
	this->excludeClassesPatterns = excludeClassesPatterns;
}

bool InstrumentationManager::instrumentationDefinitionMatches(std::shared_ptr<MethodType> methodType, std::shared_ptr<MethodInstrumentationConfig> instrConfig)
{
	// Compare method name

	if (instrConfig->getTargetMethodName().compare(methodType->getName()) != 0) {
		return false;
	}

	// Compare method parameters

	std::vector<std::wstring> methodParameters = methodType->getParameters();
	std::vector<std::wstring> targetParameters = instrConfig->getParameterTypes();

	if (targetParameters.size() != methodParameters.size()) {
		return false;
	}
	else {
		for (int i = 0; i < targetParameters.size(); i++) {
			if (targetParameters.at(i).compare(methodParameters.at(i)) != 0) {
				return false;
			}
		}
	}

	// The configuration matches

	return true;
}

void InstrumentationManager::registerInstrumentationDefinition(std::shared_ptr<ClassType> classType, std::shared_ptr<InstrumentationDefinition> instrDef)
{
	for (auto it = excludeClassesPatterns.begin(); it != excludeClassesPatterns.end(); it++) {
		if (patternMatches(*it, classType->getFqn())) {
			// Passed class is excluded
			return;
		}
	}

	auto methodTypes = classType->getMethods();
	auto instrConfigs = instrDef->getMethodInstrumentationConfigs();

	for (auto methodIt = methodTypes.begin(); methodIt != methodTypes.end(); methodIt++) {
		for (auto configIt = instrConfigs.begin(); configIt != instrConfigs.end(); ) {
			if (instrumentationDefinitionMatches(*methodIt, *configIt)) {
				sensorInstrumentationPoints.emplace((*methodIt)->getMethodDef(), (*configIt)->getSensorInstrumentationPoint());

				logger.debug("Added %i sensor points for method %ls.%ls", (*configIt)->getSensorInstrumentationPoint()->getSensorIds().size(), classType->getFqn().c_str(), (*methodIt)->getName());

				configIt = instrConfigs.erase(configIt);
			}
			else {
				configIt++;
			}
		}
	}
}

std::shared_ptr<SensorInstrumentationPoint> InstrumentationManager::getSensorInstrumentationPoint(mdMethodDef methodDef)
{
	auto it = sensorInstrumentationPoints.find(methodDef);

	if (it == sensorInstrumentationPoints.end()) {
		// Return empty pointer
		return std::shared_ptr<SensorInstrumentationPoint>();
	}
	else {
		return it->second;
	}
}

std::shared_ptr<MethodSensor> InstrumentationManager::getMethodSensorForId(JAVA_LONG sensorId)
{
	auto it = methodSensors.find(sensorId);

	if (it == methodSensors.end()) {
		auto configIt = sensorTypeConfigs.find(sensorId);
		if (configIt == sensorTypeConfigs.end()) {
			logger.error("The passed sensor id %lli is not registered!", sensorId);

			if (loggerFactory.getLogLevel() >= LEVEL_INFO) {
				logger.info("Registered sensors are:");
				for (auto cit = sensorTypeConfigs.begin(); cit != sensorTypeConfigs.end(); cit++) {
					logger.info("%lli: %ls", cit->first, cit->second->getName().c_str());
				}
			}

			return std::shared_ptr<MethodSensor>();
		}

		return methodSensorFactory.createMethodSensor(configIt->second, platformId, profilerInfo, dataSendingService);
	}
	else {
		return it->second;
	}
}
