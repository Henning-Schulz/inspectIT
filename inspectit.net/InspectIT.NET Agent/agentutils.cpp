#define WIN32_LEAN_AND_MEAN

#include "agentutils.h"
#include "java_modifiers.h"

#include <typeinfo>
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <string>

#include <cryptopp\sha.h>
#include <cryptopp\hex.h>

using namespace inspectit::logger;
using namespace inspectit::sensor;
using namespace inspectit::types;

namespace inspectit {
	namespace utils {

		std::shared_ptr<ClassType> createClassTypeFromId(ICorProfilerInfo3 *profilerInfo3, ClassID classId)
		{
			std::shared_ptr<ClassType> classType = std::make_shared<ClassType>();
			std::wstringstream hashInputStream;

			ModuleID moduleId;
			mdTypeDef typeDefToken;
			ClassID parentClassId;
			mdTypeDef parentTypeDefToken;
			ClassID typeArgs[MAX_BUFFERSIZE];
			ULONG32 typeArgsLength;

			HRESULT hr = profilerInfo3->GetClassIDInfo2(classId, &moduleId, &typeDefToken, &parentClassId, MAX_BUFFERSIZE, &typeArgsLength, typeArgs);
			if (FAILED(hr)) {
				return std::shared_ptr<ClassType>();
			}

			if (parentClassId != 0) {
				hr = profilerInfo3->GetClassIDInfo(parentClassId, 0, &parentTypeDefToken);
				if (FAILED(hr)) {
					return std::shared_ptr<ClassType>();
				}
			}

			IMetaDataImport* metaDataImport = 0;
			hr = profilerInfo3->GetModuleMetaData(moduleId, CorOpenFlags::ofRead, IID_IMetaDataImport, (LPUNKNOWN *)&metaDataImport);
			if (FAILED(hr)) {
				return std::shared_ptr<ClassType>();
			}

			// load class name

			WCHAR wcharBuffer[MAX_BUFFERSIZE];
			ULONG wcharBufferLength;
			DWORD classModifiers = 0;
			hr = metaDataImport->GetTypeDefProps(typeDefToken, wcharBuffer, MAX_BUFFERSIZE, &wcharBufferLength, &classModifiers, 0);
			if (FAILED(hr)) {
				metaDataImport->Release();
				return std::shared_ptr<ClassType>();
			}
			classType->setFqn(std::wstring(wcharBuffer, wcharBufferLength - 1)); // Omit terminating \0
			classType->setModifiers(convertClassModifiersToJava(classModifiers));
			hashInputStream << classType->getFqn() << L"{";

			// load parent class name (if available)

			if (parentClassId != 0) {
				hr = metaDataImport->GetTypeDefProps(parentTypeDefToken, wcharBuffer, MAX_BUFFERSIZE, &wcharBufferLength, 0, 0);
				if (FAILED(hr)) {
					loggerFactory.staticLog(LEVEL_WARN, "agentutils", "Could not get props of superclass %i of class %ls! Ignoring the superclass.", parentClassId, wcharBuffer);
				}
				else {
					std::wstring parentClassName(wcharBuffer, wcharBufferLength - 1);

					if (parentClassName.compare(classType->getFqn()) != 0) {
						classType->addSuperClass(std::wstring(wcharBuffer, wcharBufferLength - 1)); // Omit terminating \0
					}
					else {
						// TODO: It happened that in the CMR, the super class is the same as the considered class.
						//       This causes an endless recursion at the CMR and therefore, should be prevented.
						//       However, it is currently unclear if the reason is actual self-inheritance or
						//       a problem due to the class hashes.
						loggerFactory.staticLog(LEVEL_WARN, "agentutils", "%ls inherits from itself! Ignoring the superclass.", parentClassName);
					}
				}
			}

			// TODO: load interfaces

			/*
			HCORENUM* enumerator = 0;
			mdInterfaceImpl interfaceImpls[MAX_BUFFERSIZE];
			ULONG interfaceImplsLength;
			HRESULT enumHr;
			do {

				enumHr = metaDataImport->EnumInterfaceImpls(enumerator, typeDefToken, interfaceImpls, MAX_BUFFERSIZE, &interfaceImplsLength);

				for (size_t i = 0; i < interfaceImplsLength; i++) {

					mdToken interfaceToken;
					hr = metaDataImport->GetInterfaceImplProps(interfaceImpls[i], 0, &interfaceToken);

					if (FAILED(hr)) {
						metaDataImport->Release();
						return std::shared_ptr<ClassType>();
					}

					hr = metaDataImport->GetTypeDefProps(interfaceToken, wcharBuffer, MAX_BUFFERSIZE, &wcharBufferLength, 0, 0);
					if (FAILED(hr)) {
						metaDataImport->Release();
						return std::shared_ptr<ClassType>();
					}

					classType->addInterface(std::wstring(wcharBuffer, wcharBufferLength));
				}
			} while (enumHr == S_OK);
			*/

			// load methods

			HCORENUM enumerator = 0;
			mdMethodDef methodDefs[MAX_BUFFERSIZE];
			ULONG methodDefsLength;
			HRESULT enumHr;
			do {
				enumHr = metaDataImport->EnumMethods(&enumerator, typeDefToken, methodDefs, MAX_BUFFERSIZE, &methodDefsLength);

				for (size_t i = 0; i < methodDefsLength; i++) {
					// GetMethodProps with methodDefs[i]
					DWORD methodModifiers = 0;
					PCCOR_SIGNATURE sigBlob = NULL;
					ULONG sigBlobSize;
					hr = metaDataImport->GetMethodProps(methodDefs[i], 0, wcharBuffer, MAX_BUFFERSIZE, &wcharBufferLength, &methodModifiers, &sigBlob, &sigBlobSize, 0, 0);
					if (FAILED(hr)) {
						metaDataImport->Release();
						return std::shared_ptr<ClassType>();
					}

					// Create MethodType and add it to classType
					std::shared_ptr<MethodType> method = std::make_shared<MethodType>();
					method->setName(std::wstring(wcharBuffer, wcharBufferLength - 1)); // Omit terminating \0
					method->setModifiers(convertMethodModifiersToJava(methodModifiers));
					addMethodParams(method, metaDataImport, sigBlob, sigBlobSize);
					method->setMethodDef(methodDefs[i]);

					classType->addMethod(method);
					hashInputStream << method->getName() << L"(";
					auto params = method->getParameters();
					bool first = true;
					for (auto it = params.begin(); it != params.end(); it++) {
						if (first) {
							first = false;
						}
						else {
							hashInputStream << ",";
						}

						hashInputStream << *it;
					}
					hashInputStream << ")";
				}
			} while (enumHr == S_OK);

			hashInputStream << "}";

			metaDataImport->Release();

			classType->setHash(generateHash(hashInputStream.str()));
			return classType;
		}

		HRESULT addMethodParams(std::shared_ptr<MethodType> method, IMetaDataImport* pIMetaDataImport, PCCOR_SIGNATURE sigBlob, ULONG sigBlobSize)
		{
			WCHAR buffer[MAX_BUFFERSIZE];

			PCCOR_SIGNATURE sigBlobEnd = sigBlob + sigBlobSize;
			sigBlob += 2;

			memset(buffer, 0, MAX_BUFFERSIZE);
			sigBlob = parseMethodSignature(pIMetaDataImport, sigBlob, buffer);

			method->setReturnType(std::wstring(buffer));

			while (sigBlob < sigBlobEnd) {
				memset(buffer, 0, MAX_BUFFERSIZE);
				sigBlob = parseMethodSignature(pIMetaDataImport, sigBlob, buffer);
				method->addParameter(std::wstring(buffer));
			}

			return S_OK;
		}

		mdMethodDef getMethodDefForFunctionID(ICorProfilerInfo3 *profilerInfo3, FunctionID functionId)
		{
			mdMethodDef methodDef;
			HRESULT hr = profilerInfo3->GetFunctionInfo2(functionId, 0, 0, 0, &methodDef, 0, 0, 0);
			if (FAILED(hr)) {
				return 0;
			}

			return methodDef;
		}

		std::wstring generateHash(std::wstring input)
		{
			CryptoPP::SHA256 hash;
			byte digest[CryptoPP::SHA256::DIGESTSIZE];

			hash.CalculateDigest(digest, (byte*)input.c_str(), input.length());

			CryptoPP::HexEncoder encoder;
			std::string output;
			encoder.Attach(new CryptoPP::StringSink(output));
			encoder.Put(digest, sizeof(digest));
			encoder.MessageEnd();

			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			return converter.from_bytes(output);
		}

		PCCOR_SIGNATURE parseMethodSignature(IMetaDataImport *metaDataImport, PCCOR_SIGNATURE signature, LPWSTR signatureText)
		{
			COR_SIGNATURE corSignature = *signature++;

			switch (corSignature)
			{
			case ELEMENT_TYPE_VOID:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"void");
				break;
			case ELEMENT_TYPE_BOOLEAN:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"bool");
				break;
			case ELEMENT_TYPE_CHAR:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"wchar");
				break;
			case ELEMENT_TYPE_I1:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"int8");
				break;
			case ELEMENT_TYPE_U1:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"unsigned int8");
				break;
			case ELEMENT_TYPE_I2:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"int16");
				break;
			case ELEMENT_TYPE_U2:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"unsigned int16");
				break;
			case ELEMENT_TYPE_I4:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"int32");
				break;
			case ELEMENT_TYPE_U4:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"unsigned int32");
				break;
			case ELEMENT_TYPE_I8:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"int64");
				break;
			case ELEMENT_TYPE_U8:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"unsigned int64");
				break;
			case ELEMENT_TYPE_R4:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"float32");
				break;
			case ELEMENT_TYPE_R8:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"float64");
				break;
			case ELEMENT_TYPE_STRING:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"String");
				break;
			case ELEMENT_TYPE_VAR:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"class variable(unsigned int8)");
				break;
			case ELEMENT_TYPE_MVAR:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"method variable(unsigned int8)");
				break;
			case ELEMENT_TYPE_TYPEDBYREF:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"refany");
				break;
			case ELEMENT_TYPE_I:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"int");
				break;
			case ELEMENT_TYPE_U:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"unsigned int");
				break;
			case ELEMENT_TYPE_OBJECT:
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"Object");
				break;
			case ELEMENT_TYPE_SZARRAY:
				signature = parseMethodSignature(metaDataImport, signature, signatureText);
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"[]");
				break;
			case ELEMENT_TYPE_PINNED:
				signature = parseMethodSignature(metaDataImport, signature, signatureText);
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"pinned");
				break;
			case ELEMENT_TYPE_PTR:
				signature = parseMethodSignature(metaDataImport, signature, signatureText);
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"*");
				break;
			case ELEMENT_TYPE_BYREF:
				signature = parseMethodSignature(metaDataImport, signature, signatureText);
				wcscat_s(signatureText, MAX_BUFFERSIZE, L"&");
				break;
			case ELEMENT_TYPE_VALUETYPE:
			case ELEMENT_TYPE_CLASS:
			case ELEMENT_TYPE_CMOD_REQD:
			case ELEMENT_TYPE_CMOD_OPT:
			{
				mdToken token;
				signature += CorSigUncompressToken(signature, &token);

				WCHAR className[MAX_BUFFERSIZE];
				if (TypeFromToken(token) == mdtTypeRef)
				{
					metaDataImport->GetTypeRefProps(token, NULL, className, MAX_BUFFERSIZE, NULL);
				}
				else
				{
					metaDataImport->GetTypeDefProps(token, className, MAX_BUFFERSIZE, NULL, NULL, NULL);
				}

				wcscat_s(signatureText, MAX_BUFFERSIZE, className);
			}
			break;
			case ELEMENT_TYPE_GENERICINST:
			{
				signature = parseMethodSignature(metaDataImport, signature, signatureText);

				wcscat_s(signatureText, MAX_BUFFERSIZE, L"<");
				ULONG arguments = CorSigUncompressData(signature);
				for (ULONG i = 0; i < arguments; ++i)
				{
					if (i != 0)
					{
						wcscat_s(signatureText, MAX_BUFFERSIZE, L", ");
					}

					signature = parseMethodSignature(metaDataImport, signature, signatureText);
				}
				wcscat_s(signatureText, MAX_BUFFERSIZE, L">");
			}
			break;
			case ELEMENT_TYPE_ARRAY:
			{
				signature = parseMethodSignature(metaDataImport, signature, signatureText);
				ULONG rank = CorSigUncompressData(signature);
				if (rank == 0)
				{
					wcscat_s(signatureText, MAX_BUFFERSIZE, L"[?]");
				}
				else
				{
					ULONG arraysize = (sizeof(ULONG) * 2 * rank);
					ULONG *lower = (ULONG *)_alloca(arraysize);
					memset(lower, 0, arraysize);
					ULONG *sizes = &lower[rank];

					ULONG numsizes = CorSigUncompressData(signature);
					for (ULONG i = 0; i < numsizes && i < rank; i++)
					{
						sizes[i] = CorSigUncompressData(signature);
					}

					ULONG numlower = CorSigUncompressData(signature);
					for (ULONG i = 0; i < numlower && i < rank; i++)
					{
						lower[i] = CorSigUncompressData(signature);
					}

					wcscat_s(signatureText, MAX_BUFFERSIZE, L"[");
					for (ULONG i = 0; i < rank; ++i)
					{
						if (i > 0)
						{
							wcscat_s(signatureText, MAX_BUFFERSIZE, L",");
						}

						if (lower[i] == 0)
						{
							if (sizes[i] != 0)
							{
								WCHAR size[MAX_BUFFERSIZE];
								size[0] = '\0';
								wsprintf(size, L"%d", sizes[i]);
								wcscat_s(signatureText, MAX_BUFFERSIZE, size);
							}
						}
						else
						{
							WCHAR low[MAX_BUFFERSIZE];
							low[0] = '\0';
							wsprintf(low, L"%d", lower[i]);
							wcscat_s(signatureText, MAX_BUFFERSIZE, low);
							wcscat_s(signatureText, MAX_BUFFERSIZE, L"...");

							if (sizes[i] != 0)
							{
								WCHAR size[MAX_BUFFERSIZE];
								size[0] = '\0';
								wsprintf(size, L"%d", (lower[i] + sizes[i] + 1));
								wcscat_s(signatureText, MAX_BUFFERSIZE, size);
							}
						}
					}
					wcscat_s(signatureText, MAX_BUFFERSIZE, L"]");
				}
			}
			break;
			default:
			case ELEMENT_TYPE_END:
			case ELEMENT_TYPE_SENTINEL:
				WCHAR elementType[MAX_BUFFERSIZE];
				elementType[0] = '\0';
				wsprintf(elementType, L"<UNKNOWN:0x%X>", corSignature);
				wcscat_s(signatureText, MAX_BUFFERSIZE, elementType);
				break;
			}

			return signature;
		}

		JAVA_INT convertMethodModifiersToJava(DWORD netModifiers) {
			JAVA_INT javaModifiers = 0;

			switch (netModifiers & mdMemberAccessMask) {
			case mdPublic:
				// public
				javaModifiers |= JAVA_PUBLIC;
				break;
			case mdPrivate:
			case mdPrivateScope:
				// private
				javaModifiers |= JAVA_PRIVATE;
				break;
			default:
				// protected
				javaModifiers |= JAVA_PROTECTED;
				break;
			}

			if ((netModifiers & mdStatic) == mdStatic) {
				// static
				javaModifiers |= JAVA_STATIC;
			}

			if ((netModifiers & mdFinal) == mdFinal) {
				// final
				javaModifiers |= JAVA_FINAL;
			}

			if ((netModifiers & mdAbstract) == mdAbstract) {
				// abstract
				javaModifiers |= JAVA_ABSTRACT;
			}

			if ((netModifiers & mdPinvokeImpl) == mdPinvokeImpl) {
				// native
				javaModifiers |= JAVA_NATIVE;
			}

			return javaModifiers;
		}

		JAVA_INT convertClassModifiersToJava(DWORD netModifiers)
		{
			JAVA_INT javaModifiers = 0;

			switch (netModifiers & tdVisibilityMask) {
			case tdPublic:
			case tdNestedPublic:
				javaModifiers |= JAVA_PUBLIC;
				break;
			case tdNestedPrivate:
				javaModifiers |= JAVA_PRIVATE;
				break;
			case tdNotPublic:
				// Leave as default
				break;
			default:
				javaModifiers |= JAVA_PROTECTED;
				break;
			}

			if ((netModifiers & tdClassSemanticsMask) == tdInterface) {
				javaModifiers |= JAVA_INTERFACE;
			}

			if ((netModifiers & tdAbstract) == tdAbstract) {
				javaModifiers |= JAVA_ABSTRACT;
			}

			if ((netModifiers & tdSealed) == tdSealed) {
				javaModifiers |= JAVA_FINAL;
			}

			return javaModifiers;
		}

		void getMethodSensorClassName(std::shared_ptr<MethodSensor> sensor, LPWSTR className)
		{
			const char *typeDef = typeid(*sensor).name();
			className[0] = 0;
			swprintf_s(className, MAX_BUFFERSIZE, L"dotNet.%hs", typeDef + 6);
		}

	}
}