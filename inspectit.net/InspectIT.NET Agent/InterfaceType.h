#pragma once
#include "basehdr.h"
#include "Type.h"

#define TYPE_NAME L"Interface"

using namespace web;

class InterfaceType : Type
{
private:
	typedef Type super;

	std::vector<std::wstring> superInterfaces;

public:
	InterfaceType();
	~InterfaceType();

	std::vector<std::wstring> getSuperInterfaces();
	void addSuperInterface(std::wstring superInterface);

	std::wstring typeName();

	json::value toJson();
};

