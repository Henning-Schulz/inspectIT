#include "ClassType.h"



ClassType::ClassType()
{
}


ClassType::~ClassType()
{
}

std::vector<std::wstring> ClassType::getSuperClasses()
{
	return superClasses;
}

void ClassType::addSuperClass(std::wstring superClass)
{
	superClasses.push_back(superClass);
}

std::vector<std::wstring> ClassType::getRealizedInterfaces()
{
	return realizedInterfaces;
}

void ClassType::addInterface(std::wstring interfaceName)
{
	realizedInterfaces.push_back(interfaceName);
}

std::vector<std::shared_ptr<MethodType>> ClassType::getMethods()
{
	return methods;
}

void ClassType::addMethod(std::shared_ptr<MethodType> method)
{
	methods.push_back(method);
}

std::wstring ClassType::typeName()
{
	return TYPE_NAME;
}

json::value ClassType::toJson()
{
	json::value json = super::toJson();

	json::value superClassesArray;
	int i = 0;
	for (auto it = superClasses.begin(); it != superClasses.end(); it++) {
		superClassesArray[i] = json::value::string(*it);
		i++;
	}
	json[L"superClasses"] = superClassesArray;

	json::value interfacesArray;
	i = 0;
	for (auto it = realizedInterfaces.begin(); it != realizedInterfaces.end(); it++) {
		interfacesArray[i] = json::value::string(*it);
		i++;
	}
	json[L"realizedInterfaces"] = interfacesArray;

	json::value methodsArray;
	i = 0;
	for (auto it = methods.begin(); it != methods.end(); it++) {
		methodsArray[i] = (*it)->toJson();
		i++;
	}
	json[L"methods"] = methodsArray;

	return json;
}
