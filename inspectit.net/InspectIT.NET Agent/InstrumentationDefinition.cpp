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

std::shared_ptr<MethodInstrumentationConfig> InstrumentationDefinition::getMethodInstrumentationConfigForFunctionId(FunctionID functionId)
{
	auto it = functionIdToConfigMapping.find(functionId);

	if (it == functionIdToConfigMapping.end()) {
		return std::shared_ptr<MethodInstrumentationConfig>();
	}
	else {
		return it->second;
	}
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
