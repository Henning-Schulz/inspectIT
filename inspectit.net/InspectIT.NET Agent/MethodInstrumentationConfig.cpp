#include "MethodInstrumentationConfig.h"

using namespace inspectit::config;

MethodInstrumentationConfig::MethodInstrumentationConfig()
{
}


MethodInstrumentationConfig::~MethodInstrumentationConfig()
{
}

std::wstring MethodInstrumentationConfig::getTargetClassFqn()
{
	return targetClassFqn;
}

std::wstring MethodInstrumentationConfig::getTargetMethodName()
{
	return targetMethodName;
}

std::wstring MethodInstrumentationConfig::getReturnType()
{
	return returnType;
}

std::vector<std::wstring> MethodInstrumentationConfig::getParameterTypes()
{
	return parameterTypes;
}

std::shared_ptr<SensorInstrumentationPoint> MethodInstrumentationConfig::getSensorInstrumentationPoint()
{
	return sensorInstrumentationPoint;
}

void MethodInstrumentationConfig::fromJson(web::json::object json)
{
	targetClassFqn = json.at(L"targetClassFqn").as_string();
	targetMethodName = json.at(L"targetMethodName").as_string();
	returnType = json.at(L"returnType").as_string();

	web::json::array parameterArray = json.at(L"parameterTypes").as_array();
	for (auto it = parameterArray.begin(); it != parameterArray.end(); it++) {
		parameterTypes.push_back(it->as_string());
	}

	sensorInstrumentationPoint = std::make_shared<SensorInstrumentationPoint>();
	sensorInstrumentationPoint->fromJson(json.at(L"sensorInstrumentationPoint").as_object());
}
