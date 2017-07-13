#include "InterfaceType.h"

using namespace web;

using namespace inspectit::types;

InterfaceType::InterfaceType()
{
}


InterfaceType::~InterfaceType()
{
}

std::vector<std::wstring> InterfaceType::getSuperInterfaces()
{
	return superInterfaces;
}

void InterfaceType::addSuperInterface(std::wstring superInterface)
{
	superInterfaces.push_back(superInterface);
}

std::wstring InterfaceType::typeName()
{
	return TYPE_NAME;
}

web::json::value InterfaceType::toJson()
{
	web::json::value json = super::toJson();

	web::json::value interfArray;
	int i = 0;
	for (auto it = superInterfaces.begin(); it != superInterfaces.end(); it++) {
		interfArray[i] = web::json::value::string(*it);
		i++;
	}

	return json;
}
