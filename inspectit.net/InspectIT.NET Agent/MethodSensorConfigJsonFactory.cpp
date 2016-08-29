#include "MethodSensorConfigJsonFactory.h"

#include "StackTraceSensorConfig.h"
#include "TimerSensorConfig.h"



MethodSensorConfigJsonFactory::MethodSensorConfigJsonFactory()
{
}


MethodSensorConfigJsonFactory::~MethodSensorConfigJsonFactory()
{
}

std::shared_ptr<MethodSensorConfig> MethodSensorConfigJsonFactory::createFromJson(json::object jsonObject)
{
	utility::string_t className = jsonObject.at(L"className").as_string();
	logger.debug("className is %ls", className.c_str());

	if (className.compare(StackTraceSensorConfig::CLASS_NAME) == 0) {
		logger.debug("Creating %ls...", StackTraceSensorConfig::CLASS_NAME);
		std::shared_ptr<StackTraceSensorConfig> config = std::make_shared<StackTraceSensorConfig>();
		utility::string_t priorityName = jsonObject.at(L"priority").as_string();
		auto prioIt = methodSensorPriorities.find(priorityName.c_str());
		if (prioIt != methodSensorPriorities.end()) {
			config->setPriority(prioIt->second);
		}
		else {
			logger.error("Could not find priority %ls", priorityName.c_str());
			return std::shared_ptr<StackTraceSensorConfig>();
		}
		
		utility::string_t triggerTypeName = jsonObject.at(L"triggerType").as_string();
		auto triggerIt = triggerTypes.find(triggerTypeName.c_str());
		if (triggerIt != triggerTypes.end()) {
			config->setTriggerType(triggerIt->second);
			logger.debug("Using trigger %ls\n", triggerTypeName.c_str());
		}
		else {
			logger.error("Could not find trigger type %ls", triggerTypeName.c_str());
		}
		utility::string_t providerTypeName = jsonObject.at(L"providerType").as_string();
		auto providerIt = providerTypes.find(providerTypeName.c_str());
		if (providerIt != providerTypes.end()) {
			config->setProviderType(providerIt->second);
		}
		else {
			logger.error("Could not find provider type %ls", providerTypeName.c_str());
		}

		JAVA_LONG samplingInterval = jsonObject.at(L"samplingInterval").as_number().to_int64();
		config->setSamplingInterval(samplingInterval);
		logger.debug("Created.");
		return config;
	}
	else if (className.compare(TimerSensorConfig::CLASS_NAME) == 0) {
		logger.debug("Creating %ls...", TimerSensorConfig::CLASS_NAME);
		std::shared_ptr<MethodSensorConfig> config = std::make_shared<TimerSensorConfig>();
		return config;
	}
	else {
		return std::shared_ptr<StackTraceSensorConfig>();
	}
}
