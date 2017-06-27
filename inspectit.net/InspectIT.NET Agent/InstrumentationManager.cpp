#include "InstrumentationManager.h"

#include "stringutils.h"
#include "MethodSensorFactory.h"


InstrumentationManager::InstrumentationManager(JAVA_LONG platformId, ICorProfilerInfo3 *profilerInfo)
{
	this->platformId = platformId;
	this->profilerInfo = profilerInfo;
}


InstrumentationManager::~InstrumentationManager()
{
}

void InstrumentationManager::setExcludeClassesPatterns(std::vector<std::wstring> excludeClassesPatterns)
{
	this->excludeClassesPatterns = excludeClassesPatterns;
}

bool InstrumentationManager::instrumentationDefinitionMatches(std::shared_ptr<MethodType> methodType, std::shared_ptr<MethodInstrumentationConfig> instrConfig)
{
	// Compare method name

	if (instrConfig->getTargetClassFqn().compare(methodType->getName()) != 0) {
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
				sensorInstrumentationPoints.emplace((*methodIt)->getFunctionId(), (*configIt)->getSensorInstrumentationPoint());

				configIt = instrConfigs.erase(configIt);
			}
			else {
				configIt++;
			}
		}
	}
}

std::shared_ptr<SensorInstrumentationPoint> InstrumentationManager::getSensorInstrumentationPoint(FunctionID functionId)
{
	auto it = sensorInstrumentationPoints.find(functionId);

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
			// TODO: Error!
			return std::shared_ptr<MethodSensor>();
		}

		return methodSensorFactory.createMethodSensor(configIt->second, platformId, profilerInfo);
	}
	else {
		return it->second;
	}
}
