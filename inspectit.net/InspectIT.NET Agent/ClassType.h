#pragma once
#include "Type.h"
#include "MethodType.h"

#define TYPE_NAME L"Class"

using namespace web;

class ClassType : public Type
{
private:
	typedef Type super;

	std::vector<std::wstring> superClasses;
	std::vector<std::wstring> realizedInterfaces;

	std::vector<MethodType> methods;

public:
	ClassType();
	~ClassType();

	std::vector<std::wstring> getSuperClasses();
	void addSuperClass(std::wstring superClass);
	std::vector<std::wstring> getRealizedInterfaces();
	void addInterface(std::wstring interfaceName);

	std::vector<MethodType> getMethods();
	void addMethod(MethodType method);

	std::wstring typeName();

	json::value toJson();
};

