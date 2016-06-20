#pragma once

#include "basehdr.h"

#include <vector>

class MethodSensorAssignment
{
private:
	const wchar_t* sensorClassName;

	std::wstring className;
	bool superclass;
	bool interf;
	std::wstring methodName;
	bool parametersSpecified;
	std::vector<std::wstring> parameters;
	bool constructor;
	bool publicModifier;
	bool protectedModifier;
	bool privateModifier;
	bool defaultModifier;

public:
	MethodSensorAssignment();
	~MethodSensorAssignment();

	const wchar_t* getSensorClassName();
	void setSensorClassName(const wchar_t* sensorClassName);

	std::wstring getClassName();
	void setClassName(std::wstring className);
	bool isSuperclass();
	void setSuperclass(bool superclass);
	bool isInterface();
	void setInterface(bool interf);
	std::wstring getMethodName();
	void setMethodName(std::wstring methodName);
	bool areParametersSpecified();
	void setParametersSpecified(bool parametersSpecified);
	std::vector<std::wstring> getParameters();
	void setParameters(std::vector<std::wstring> parameters);
	bool isConstructor();
	void setConstructor(bool constructor);
	bool isPublicModifier();
	void setPublicModifier(bool publicModifier);
	bool isProtectedModifier();
	void setProtectedModifier(bool protectedModifier);
	bool isPrivateModifier();
	void setPrivateModifier(bool privateModifier);
	bool isDefaultModifier();
	void setDefaultModifier(bool defaultModifier);
};
