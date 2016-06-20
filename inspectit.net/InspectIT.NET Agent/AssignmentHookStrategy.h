#pragma once

#include "HookStrategy.h"
#include "MethodSensorAssignment.h"

#include <memory>

class AssignmentHookStrategy : public HookStrategy
{
private:
	std::vector<std::shared_ptr<MethodSensorAssignment>> assignments;

public:
	AssignmentHookStrategy();
	~AssignmentHookStrategy();

	bool hook(LPWSTR className, LPWSTR methodName, std::vector<LPWSTR> parameters, JAVA_INT modifiers);

	void addAssignment(std::shared_ptr<MethodSensorAssignment> ass);
};

