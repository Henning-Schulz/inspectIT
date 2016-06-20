#pragma once

#include "basehdr.h"
#include "MethodSensor.h"

#include <vector>
#include <memory>

PCCOR_SIGNATURE parseMethodSignature(IMetaDataImport *metaDataImport, PCCOR_SIGNATURE signature, LPWSTR signatureText);

JAVA_INT convertMethodModifiersToJava(DWORD netModifiers);

void getMethodSensorClassName(std::shared_ptr<MethodSensor> sensor, LPWSTR className);

std::vector<std::string> __cdecl getAllDefinedIPs();