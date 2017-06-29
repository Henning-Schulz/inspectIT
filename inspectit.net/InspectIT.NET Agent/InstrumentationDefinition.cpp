#include "InstrumentationDefinition.h"



InstrumentationDefinition::InstrumentationDefinition()
{
}


InstrumentationDefinition::~InstrumentationDefinition()
{
}

std::wstring InstrumentationDefinition::getClassName()
{
	return className;
}

std::vector<std::shared_ptr<MethodInstrumentationConfig>> InstrumentationDefinition::getMethodInstrumentationConfigs()
{
	return methodInstrumentationConfigs;
}

void InstrumentationDefinition::fromJson(json::object json)
{
	className = json.at(L"className").as_string();

	json::array instrConfigArray = json.at(L"methodInstrumentationConfigs").as_array();
	for (auto it = instrConfigArray.begin(); it != instrConfigArray.end(); it++) {
		auto instrumentationConfig = std::make_shared<MethodInstrumentationConfig>();
		instrumentationConfig->fromJson(it->as_object());
		methodInstrumentationConfigs.push_back(instrumentationConfig);
	}
}
