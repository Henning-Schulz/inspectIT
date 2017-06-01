#pragma once
#include "JsonReceivable.h"
#include "MethodInstrumentationConfig.h"

#include <map>

class InstrumentationDefinition :
	public JsonReceivable
{
private:
	std::wstring className;
	std::vector<std::shared_ptr<MethodInstrumentationConfig>> methodInstrumentationConfigs;

	std::map<FunctionID, std::shared_ptr<MethodInstrumentationConfig>> functionIdToConfigMapping;

public:
	InstrumentationDefinition();
	~InstrumentationDefinition();

	std::wstring getClassName();
	std::vector<std::shared_ptr<MethodInstrumentationConfig>> getMethodInstrumentationConfigs();

	std::shared_ptr<MethodInstrumentationConfig> getMethodInstrumentationConfigForFunctionId(FunctionID functionId);

	void fromJson(json::object json);
};

