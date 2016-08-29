#pragma once

#include "MethodSensorConfig.h"
#include "cmp_wstr.h"

enum TriggerType { TIMER, RANDOMIZED_TIMER, STACK_SIZE_BASED };
static std::map<const wchar_t*, TriggerType, cmp_wstr> triggerTypes({ { L"TIMER", TIMER },{ L"RANDOMIZED_TIMER", RANDOMIZED_TIMER },{ L"STACK_SIZE_BASED", STACK_SIZE_BASED } });
enum ProviderType { NATIVE, SHADOW };
static std::map<const wchar_t*, ProviderType, cmp_wstr> providerTypes({ { L"NATIVE", NATIVE },{ L"SHADOW", SHADOW } });

class StackTraceSensorConfig : public MethodSensorConfig
{
private:
	MethodSensorPriority priority;

	TriggerType triggerType;
	ProviderType providerType;
	JAVA_LONG samplingInterval;

public:
	static const LPWSTR CLASS_NAME;
	static const LPWSTR SENSOR_NAME;

	StackTraceSensorConfig();
	~StackTraceSensorConfig();

	LPWSTR getClassName();
	LPWSTR getName();
	MethodSensorPriority getPriority();
	void setPriority(MethodSensorPriority priority);

	TriggerType getTriggerType();
	void setTriggerType(TriggerType triggerType);

	ProviderType getProviderType();
	void setProviderType(ProviderType providerType);

	JAVA_LONG getSamplingInterval();
	void setSamplingInterval(JAVA_LONG samplingInterval);
};

