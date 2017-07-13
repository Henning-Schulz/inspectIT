#include "InstrumentationDefinition.h"

using namespace inspectit::config;

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

void InstrumentationDefinition::fromJson(web::json::object json)
{
	className = json.at(L"className").as_string();

	web::json::array instrConfigArray = json.at(L"methodInstrumentationConfigs").as_array();
	for (auto it = instrConfigArray.begin(); it != instrConfigArray.end(); it++) {
		auto instrumentationConfig = std::make_shared<MethodInstrumentationConfig>();
		instrumentationConfig->fromJson(it->as_object());
		methodInstrumentationConfigs.push_back(instrumentationConfig);
	}
}
