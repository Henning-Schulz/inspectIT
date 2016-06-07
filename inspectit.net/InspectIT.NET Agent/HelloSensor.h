#pragma once

#include "MethodSensor.h"
#include "Logger.h"

class HelloSensor :	public MethodSensor
{
private:
	Logger logger = loggerFactory.createLogger("HelloSensor");

public:
	HelloSensor();
	~HelloSensor();

	void beforeBody(METHOD_ID methodID);
	void afterBody(METHOD_ID methodID);

};

