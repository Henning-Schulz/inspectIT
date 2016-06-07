#pragma once

#include "basehdr.h"
#include "MethodSensor.h"

#include <vector>

PCCOR_SIGNATURE parseMethodSignature(IMetaDataImport *metaDataImport, PCCOR_SIGNATURE signature, LPWSTR signatureText);

JAVA_INT convertMethodModifiersToJava(DWORD netModifiers);

void getMethodSensorClassName(MethodSensor *sensor, LPWSTR className);

std::vector<std::string> __cdecl getAllDefinedIPs();