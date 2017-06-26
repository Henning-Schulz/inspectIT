#include "MethodType.h"



MethodType::MethodType()
{
}


MethodType::~MethodType()
{
}

std::wstring MethodType::getName()
{
	return name;
}

void MethodType::setName(std::wstring name)
{
	this->name = name;
}

JAVA_INT MethodType::getModifiers()
{
	return modifiers;
}

void MethodType::setModifiers(JAVA_INT modifiers)
{
	this->modifiers = modifiers;
}

std::wstring MethodType::getReturnType()
{
	return returnType;
}

void MethodType::setReturnType(std::wstring returnType)
{
	this->returnType = returnType;
}

std::vector<std::wstring> MethodType::getParameters()
{
	return parameters;
}

void MethodType::addParameter(std::wstring parameter)
{
	parameters.push_back(parameter);
}

std::vector<std::wstring> MethodType::getExceptions()
{
	return std::vector<std::wstring>();
}

void MethodType::addException(std::wstring exception)
{
	exceptions.push_back(exception);
}

std::vector<std::wstring> MethodType::getAnnotations()
{
	return std::vector<std::wstring>();
}

void MethodType::addAnnotation(std::wstring annotation)
{
	annotations.push_back(annotation);
}

FunctionID MethodType::getFunctionId()
{
	return functionId;
}

void MethodType::setFunctionId(FunctionID functionId)
{
	this->functionId = functionId;
}

json::value MethodType::toJson()
{
	json::value json;

	json[L"name"] = json::value::string(name);
	json[L"modifiers"] = json::value::number(modifiers);
	json[L"returnType"] = json::value::string(returnType);

	json::value parametersArray;
	int i = 0;
	for (auto it = parameters.begin(); it != parameters.end(); it++) {
		parametersArray[i] = json::value::string(*it);
		i++;
	}
	json[L"parameters"] = parametersArray;

	json::value exceptionsArray;
	i = 0;
	for (auto it = exceptions.begin(); it != exceptions.end(); it++) {
		exceptionsArray[i] = json::value::string(*it);
		i++;
	}
	json[L"exceptions"] = exceptionsArray;

	json::value annotationsArray;
	i = 0;
	for (auto it = annotations.begin(); it != annotations.end(); it++) {
		annotationsArray[i] = json::value::string(*it);
		i++;
	}
	json[L"annotations"] = annotationsArray;

	return json;
}