#include "AssignmentHookStrategy.h"

#include "StackTraceSensorConfig.h"
#include "stringutils.h"



AssignmentHookStrategy::AssignmentHookStrategy()
{
}


AssignmentHookStrategy::~AssignmentHookStrategy()
{
}

bool AssignmentHookStrategy::hook(LPWSTR className, LPWSTR methodName, std::vector<LPWSTR> parameters, JAVA_INT modifiers)
{
	for (auto it = excludedClasses.begin(); it != excludedClasses.end(); it++) {
		if (patternMatches(*it, className)) {
			return false;
		}
	}

	for (auto it = assignments.begin(); it != assignments.end(); it++) {
		auto ass = *it;
		bool hook = true;

		if (!patternMatches(ass->getClassName(), className)) {
			hook = false;
		}
		else if (ass->isConstructor()) {
			hook = patternMatches(L".ctor", methodName);
		}
		else if (patternMatches(L".ctor", methodName)) {
			// Method is constructor
			hook = false;
		}
		else if (!patternMatches(ass->getMethodName(), methodName)) {
			hook = false;
		}

		if (ass->areParametersSpecified()) {
			std::vector<std::wstring> paramPatterns = ass->getParameters();

			if (paramPatterns.size() != parameters.size()) {
				hook = false;
			}
			else {
				for (int i = 0; i < parameters.size(); i++) {
					if (!patternMatches(paramPatterns.at(i), parameters.at(i))) {
						hook = false;
						break;
					}
				}
			}
		}

		// compare modifiers
		if ((modifiers & 0x1) && !ass->isPublicModifier()) {
			hook = false;
		} else if ((modifiers & 0x2) && !ass->isPrivateModifier()) {
			hook = false;
		} else if ((modifiers & 0x4) && !ass->isProtectedModifier()) {
			hook = false;
		}

		if (hook) {
			return true;
		}
	}

	return false;
}

void AssignmentHookStrategy::addAssignment(std::shared_ptr<MethodSensorAssignment> ass)
{
	if (ass) {
		assignments.push_back(ass);
	}
}

boolean AssignmentHookStrategy::isEmpty()
{
	return assignments.empty();
}

void AssignmentHookStrategy::setExcludedClasses(std::vector<std::wstring> excludedClasses)
{
	this->excludedClasses = excludedClasses;
}
