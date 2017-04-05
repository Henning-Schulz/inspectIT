#pragma once
#include "JsonReceivable.h"
#include "SensorInstrumentationPoint.h"

class MethodInstrumentationConfig :
	public JsonReceivable
{
private:
	std::wstring targetClassFqn;
	std::wstring targetMethodName;
	std::wstring returnType;
	std::vector<std::wstring> parameterTypes;

	std::shared_ptr<SensorInstrumentationPoint> sensorInstrumentationPoint;
	// TODO: sepcialInstrumentationPoint

public:
	MethodInstrumentationConfig();
	~MethodInstrumentationConfig();

	std::wstring getTargetClassFqn();
	std::wstring getTargetMethodName();
	std::wstring getReturnType();
	std::vector<std::wstring> getParameterTypes();

	std::shared_ptr<SensorInstrumentationPoint> getSensorInstrumentationPoint();

	void fromJson(json::object json);
};

