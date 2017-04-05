#include "InterfaceType.h"



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

json::value InterfaceType::toJson()
{
	json::value json = super::toJson();

	json::value interfArray;
	int i = 0;
	for (auto it = superInterfaces.begin(); it != superInterfaces.end(); it++) {
		interfArray[i] = json::value::string(*it);
		i++;
	}

	return json;
}
