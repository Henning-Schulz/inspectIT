#include "MethodType.h"

using namespace web;

using namespace inspectit::types;

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

mdMethodDef MethodType::getMethodDef()
{
	return methodDef;
}

void MethodType::setMethodDef(mdMethodDef methodDef)
{
	this->methodDef = methodDef;
}

json::value MethodType::toJson()
{
	web::json::value json;

	json[L"name"] = web::json::value::string(name);
	json[L"modifiers"] = web::json::value::number(modifiers);
	json[L"returnType"] = web::json::value::string(returnType);

	web::json::value parametersArray;
	int i = 0;
	for (auto it = parameters.begin(); it != parameters.end(); it++) {
		parametersArray[i] = web::json::value::string(*it);
		i++;
	}
	json[L"parameters"] = parametersArray;

	web::json::value exceptionsArray;
	i = 0;
	for (auto it = exceptions.begin(); it != exceptions.end(); it++) {
		exceptionsArray[i] = web::json::value::string(*it);
		i++;
	}
	json[L"exceptions"] = exceptionsArray;

	web::json::value annotationsArray;
	i = 0;
	for (auto it = annotations.begin(); it != annotations.end(); it++) {
		annotationsArray[i] = web::json::value::string(*it);
		i++;
	}
	json[L"annotations"] = annotationsArray;

	return json;
}
