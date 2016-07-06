#pragma once

#include "basehdr.h"
#include "MethodSensor.h"

#include <vector>
#include <memory>

BOOL getSpecificsOfMethod(ICorProfilerInfo *profilerInfo, FunctionID functionID, LPWSTR wszClass, LPWSTR wszMethod, LPWSTR returnType,
	JAVA_INT *javaModifiers, std::vector<LPWSTR> *parameterTypes);

PCCOR_SIGNATURE parseMethodSignature(IMetaDataImport *metaDataImport, PCCOR_SIGNATURE signature, LPWSTR signatureText);

JAVA_INT convertMethodModifiersToJava(DWORD netModifiers);

void getMethodSensorClassName(std::shared_ptr<MethodSensor> sensor, LPWSTR className);

std::vector<std::string> __cdecl getAllDefinedIPs();