#include "MethodSensorAssignment.h"



MethodSensorAssignment::MethodSensorAssignment()
{
}


MethodSensorAssignment::~MethodSensorAssignment()
{
}

const wchar_t* MethodSensorAssignment::getSensorClassName()
{
	return sensorClassName;
}

void MethodSensorAssignment::setSensorClassName(const wchar_t* sensorClassName)
{
	this->sensorClassName = sensorClassName;
}

std::wstring MethodSensorAssignment::getClassName()
{
	return className;
}

void MethodSensorAssignment::setClassName(std::wstring className)
{
	this->className = className;
}

bool MethodSensorAssignment::isSuperclass()
{
	return superclass;
}

void MethodSensorAssignment::setSuperclass(bool superclass)
{
	this->superclass = superclass;
}

bool MethodSensorAssignment::isInterface()
{
	return interf;
}

void MethodSensorAssignment::setInterface(bool interf)
{
	this->interf = interf;
}

std::wstring MethodSensorAssignment::getMethodName()
{
	return methodName;
}

void MethodSensorAssignment::setMethodName(std::wstring methodName)
{
	this->methodName = methodName;
}

bool MethodSensorAssignment::areParametersSpecified()
{
	return parametersSpecified;
}

void MethodSensorAssignment::setParametersSpecified(bool parametersSpecified)
{
	this->parametersSpecified = parametersSpecified;
}

std::vector<std::wstring> MethodSensorAssignment::getParameters()
{
	return parameters;
}

void MethodSensorAssignment::setParameters(std::vector<std::wstring> parameters)
{
	this->parameters = parameters;
	this->setParametersSpecified(true);
}

bool MethodSensorAssignment::isConstructor()
{
	return constructor;
}

void MethodSensorAssignment::setConstructor(bool constructor)
{
	this->constructor = constructor;
}

bool MethodSensorAssignment::isPublicModifier()
{
	return publicModifier;
}

void MethodSensorAssignment::setPublicModifier(bool publicModifier)
{
	this->publicModifier = publicModifier;
}

bool MethodSensorAssignment::isProtectedModifier()
{
	return protectedModifier;
}

void MethodSensorAssignment::setProtectedModifier(bool protectedModifier)
{
	this->protectedModifier = protectedModifier;
}

bool MethodSensorAssignment::isPrivateModifier()
{
	return privateModifier;
}

void MethodSensorAssignment::setPrivateModifier(bool privateModifier)
{
	this->privateModifier = privateModifier;
}

bool MethodSensorAssignment::isDefaultModifier()
{
	return defaultModifier;
}

void MethodSensorAssignment::setDefaultModifier(bool defaultModifier)
{
	this->defaultModifier = defaultModifier;
}
