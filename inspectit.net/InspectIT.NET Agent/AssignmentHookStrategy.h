#pragma once

#include "HookStrategy.h"
#include "MethodSensorAssignment.h"
#include "Logger.h"

#include <memory>
#include <list>

class AssignmentHookStrategy : public HookStrategy
{
private:
	std::vector<std::shared_ptr<MethodSensorAssignment>> assignments;
	std::vector<std::wstring> excludedClasses;

	Logger logger = loggerFactory.createLogger("AssignmentHookStrategy");

public:
	AssignmentHookStrategy();
	~AssignmentHookStrategy();

	bool hook(LPWSTR className, LPWSTR methodName, std::vector<LPWSTR> parameters, JAVA_INT modifiers);

	void addAssignment(std::shared_ptr<MethodSensorAssignment> ass);
	boolean isEmpty();

	void setExcludedClasses(std::vector<std::wstring> excludedClasses);
};

