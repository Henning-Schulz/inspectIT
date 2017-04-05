#pragma once
#include "AbstractSensorTypeConfig.h"
class PlatformSensorTypeConfig :
	public AbstractSensorTypeConfig
{
private:
	typedef AbstractSensorTypeConfig super;

public:
	PlatformSensorTypeConfig();
	~PlatformSensorTypeConfig();

	void fromJson(json::object json);
};

