#include "StackTraceSensorAssignment.h"



StackTraceSensorAssignment::StackTraceSensorAssignment()
{
}


StackTraceSensorAssignment::~StackTraceSensorAssignment()
{
}

void StackTraceSensorAssignment::setBaseMethodAssignment(std::shared_ptr<MethodSensorAssignment> baseMethodAssignment)
{
	this->baseMethodAssignment = baseMethodAssignment;
}

std::shared_ptr<MethodSensorAssignment> StackTraceSensorAssignment::getBaseMethodAssignment()
{
	return baseMethodAssignment;
}