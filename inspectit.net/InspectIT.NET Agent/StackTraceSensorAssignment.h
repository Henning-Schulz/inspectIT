#pragma once

#include "MethodSensorAssignment.h"

#include <memory>

class StackTraceSensorAssignment : public MethodSensorAssignment
{
private:
	std::shared_ptr<MethodSensorAssignment> baseMethodAssignment;

public:
	StackTraceSensorAssignment();
	~StackTraceSensorAssignment();

	void setBaseMethodAssignment(std::shared_ptr<MethodSensorAssignment> baseMethodAssignment);
	std::shared_ptr<MethodSensorAssignment> getBaseMethodAssignment();
};

