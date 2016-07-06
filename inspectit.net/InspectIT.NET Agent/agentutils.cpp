#define WIN32_LEAN_AND_MEAN

#include "agentutils.h"

#include <typeinfo>
#include <iostream>
#include <sstream>

#include <winsock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>

// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Ws2_32.lib");

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


BOOL getSpecificsOfMethod(ICorProfilerInfo *profilerInfo, FunctionID functionID, LPWSTR wszClass, LPWSTR wszMethod, LPWSTR returnType,
	JAVA_INT *javaModifiers, std::vector<LPWSTR> *parameterTypes) {

	BOOL succ = 0;

	IMetaDataImport* pIMetaDataImport = 0;
	HRESULT hr = S_OK;
	mdToken funcToken = 0;
	WCHAR szFunction[MAX_BUFFERSIZE];
	char str[MAX_BUFFERSIZE];

	WCHAR szClass[MAX_BUFFERSIZE];

	// get the token for the function which we will use to get its name
	hr = profilerInfo->GetTokenAndMetaDataFromFunction(functionID, IID_IMetaDataImport, (LPUNKNOWN *)&pIMetaDataImport, &funcToken);

	if (SUCCEEDED(hr))
	{
		mdTypeDef classTypeDef;
		ULONG cchFunction;
		ULONG cchClass;
		DWORD methodModifiers = 0;
		PCCOR_SIGNATURE sigBlob = NULL;
		ULONG sigBlobSize;

		// retrieve the function properties based on the token
		hr = pIMetaDataImport->GetMethodProps(funcToken, &classTypeDef, szFunction, MAX_BUFFERSIZE, &cchFunction, &methodModifiers, &sigBlob, &sigBlobSize, 0, 0);
		if (SUCCEEDED(hr))
		{
			// get the function name
			hr = pIMetaDataImport->GetTypeDefProps(classTypeDef, szClass, MAX_BUFFERSIZE, &cchClass, 0, 0);

			if (SUCCEEDED(hr))
			{
				wcscpy_s(wszMethod, MAX_BUFFERSIZE, szFunction);
				wcscpy_s(wszClass, MAX_BUFFERSIZE, szClass);

				*javaModifiers = convertMethodModifiersToJava(methodModifiers);

				// Structure of sigBlob:
				// first and second byte: ???
				// following bytes: return type (one or several bytes; depending on type)
				// after return type: parameter types

				PCCOR_SIGNATURE sigBlobEnd = sigBlob + sigBlobSize;
				sigBlob += 2;

				memset(returnType, 0, MAX_BUFFERSIZE);
				sigBlob = parseMethodSignature(pIMetaDataImport, sigBlob, returnType);

				while (sigBlob < sigBlobEnd) {
					LPWSTR param = new WCHAR[MAX_BUFFERSIZE];
					memset(param, 0, MAX_BUFFERSIZE);
					sigBlob = parseMethodSignature(pIMetaDataImport, sigBlob, param);
					parameterTypes->push_back(param);
				}

				succ = 1;
			}
		}

		// release our reference to the metadata
		pIMetaDataImport->Release();
	}

	return succ;
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
		javaModifiers |= 0x1;
		break;
	case mdPrivate:
	case mdPrivateScope:
		// private
		javaModifiers |= 0x2;
		break;
	default:
		// protected
		javaModifiers |= 0x4;
		break;
	}

	if ((netModifiers & mdStatic) == mdStatic) {
		// static
		javaModifiers |= 0x8;
	}

	if ((netModifiers & mdFinal) == mdFinal) {
		// final
		javaModifiers |= 0x10;
	}

	if ((netModifiers & mdAbstract) == mdAbstract) {
		// abstract
		javaModifiers |= 0x400;
	}

	if((netModifiers & mdPinvokeImpl) == mdPinvokeImpl) {
		// native
		javaModifiers |= 0x100;
	}

	return javaModifiers;
}

void getMethodSensorClassName(std::shared_ptr<MethodSensor> sensor, LPWSTR className)
{
	const char *typeDef = typeid(*sensor).name();
	className[0] = 0;
	swprintf_s(className, MAX_BUFFERSIZE, L"dotNet.%hs", typeDef + 6);
}

std::vector<std::string> __cdecl getAllDefinedIPs()
{
	/* Declare and initialize variables */

	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i = 0;

	// Set the flags to pass to GetAdaptersAddresses
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

	// default to unspecified address family (both)
	ULONG family = AF_UNSPEC;

	LPVOID lpMsgBuf = NULL;

	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	ULONG Iterations = 0;

	char buff[100];
	DWORD bufflen = 100;

	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
	PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
	PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
	PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
	IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
	IP_ADAPTER_PREFIX *pPrefix = NULL;

	std::vector<std::string> ips;

	// Allocate a 15 KB buffer to start with.
	outBufLen = WORKING_BUFFER_SIZE;

	do {

		pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);
		if (pAddresses == NULL) {
			printf("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
			exit(1);
		}

		dwRetVal =
			GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

		if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
			FREE(pAddresses);
			pAddresses = NULL;
		}
		else {
			break;
		}

		Iterations++;

	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

	if (dwRetVal == NO_ERROR) {
		// If successful, output some information from the data we received
		pCurrAddresses = pAddresses;
		while (pCurrAddresses) {

			pUnicast = pCurrAddresses->FirstUnicastAddress;
			if (pUnicast != NULL) {
				for (i = 0; pUnicast != NULL; i++) {
					SOCKET_ADDRESS address = pUnicast->Address;
					if (address.lpSockaddr->sa_family == AF_INET) {
						sockaddr_in *sa_in = (sockaddr_in *)pUnicast->Address.lpSockaddr;
						PCSTR ip = inet_ntop(AF_INET, &(sa_in->sin_addr), buff, bufflen);
						ips.push_back(std::string(ip));
					}
					else if (address.lpSockaddr->sa_family == AF_INET6) {
						sockaddr_in6 *sa_in6 = (sockaddr_in6 *)pUnicast->Address.lpSockaddr;
						PCSTR ip = inet_ntop(AF_INET6, &(sa_in6->sin6_addr), buff, bufflen);
						ips.push_back(std::string(ip));
					}

					pUnicast = pUnicast->Next;
				}
			}

			pCurrAddresses = pCurrAddresses->Next;
		}
	}
	else {
		printf("Call to GetAdaptersAddresses failed with error: %d\n", dwRetVal);
		if (dwRetVal == ERROR_NO_DATA)
			printf("\tNo addresses were found for the requested parameters\n");
		else {
			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				// Default language
				(LPTSTR)& lpMsgBuf, 0, NULL)) {
				printf("\tError: %s", lpMsgBuf);
				LocalFree(lpMsgBuf);
				if (pAddresses)
					FREE(pAddresses);
			}
		}
	}

	if (pAddresses) {
		FREE(pAddresses);
	}

	return ips;
}