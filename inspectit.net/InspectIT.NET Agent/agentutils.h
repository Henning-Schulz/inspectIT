#pragma once

#include "basehdr.h"
#include "MethodSensor.h"
#include "ClassType.h"
#include "MethodType.h"

#include <vector>
#include <memory>

PCCOR_SIGNATURE parseMethodSignature(IMetaDataImport *metaDataImport, PCCOR_SIGNATURE signature, LPWSTR signatureText);

JAVA_INT convertMethodModifiersToJava(DWORD netModifiers);
JAVA_INT convertClassModifiersToJava(DWORD netModifiers);

void getMethodSensorClassName(std::shared_ptr<MethodSensor> sensor, LPWSTR className);

HRESULT addMethodParams(std::shared_ptr<MethodType> method, IMetaDataImport* pIMetaDataImport, PCCOR_SIGNATURE sigBlob, ULONG sigBlobSize);

std::shared_ptr<ClassType> createClassTypeFromId(ICorProfilerInfo3 *profilerInfo3, ClassID classId);

mdMethodDef getMethodDefForFunctionID(ICorProfilerInfo3 *profilerInfo3, FunctionID functionId);

std::wstring generateHash(std::wstring input);