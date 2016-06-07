#pragma once

#include "MethodSensor.h"

class ShadowStackSensor : public MethodSensor
{
public:
	ShadowStackSensor();
	~ShadowStackSensor();

	void beforeBody(FunctionID functionID);
	void afterBody(FunctionID functionID);
};

