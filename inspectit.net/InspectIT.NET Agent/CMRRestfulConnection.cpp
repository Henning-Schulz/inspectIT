#include "CMRRestfulConnection.h"

#include <exception>

using namespace web::http;
using namespace web::http::client;
using namespace web;

CMRConnection* cmrConnection;

bool setupCMRConnection() {
	cmrConnection = new CMRRestfulConnection();

	return ((CMRRestfulConnection*)cmrConnection)->testConnection();
}

void shutdownCMRConnection() {
	delete cmrConnection;
}

CMRRestfulConnection::CMRRestfulConnection()
{
	baseUrl = L"http://localhost:8182/rest";
	registrationPath = L"/dotNetRegistration";
	connectionTestPath = L"/dotNetConnection/test";
	storagePath = L"/dotNetAgentStorage";
	logger.debug("Created connection to %ls", baseUrl);
}


CMRRestfulConnection::~CMRRestfulConnection()
{
}

bool CMRRestfulConnection::testConnection() {
	std::wstringstream urlStream;
	urlStream << baseUrl << connectionTestPath;

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		response = client.request(methods::GET).get();
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return false;
	}

	status_code c = response.status_code();
	if (c == status_codes::OK) {
		return true;
	}
	else {
		logger.error("Connection test failed. Response was %u!", c);
		return false;
	}
}

/*
 * The agentName and the version should not contain whitespaces. Use %20 instead.
 */
JAVA_LONG CMRRestfulConnection::registerPlatform(std::vector<std::string> definedIps, LPWSTR agentName, LPWSTR version)
{
	logger.debug("Registering platform...");

	std::wstringstream urlStream;
	urlStream << baseUrl << registrationPath << "/registerPlatformIdent";

	json::value ipArray;

	int i = 0;
	for (std::vector<std::string>::iterator it = definedIps.begin(); it != definedIps.end(); it++) {
		std::wstringstream wss;
		wss << it->c_str();
		ipArray[i] = json::value(wss.str());
		i++;
	}

	std::wstringstream argsStream;
	argsStream << "?agentName=" << agentName << "&version=" << version;

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		response = client.request(methods::POST, argsStream.str().c_str(), ipArray.serialize(), MIME_JSON).get();
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return -1;
	}
	
	status_code c = response.status_code();
	if (c == status_codes::OK) {
		auto body = response.extract_string();
		JAVA_LONG id = _wtoi64(body.get().c_str());
		logger.debug("Platform registered. ID is %lli.", id);
		return id;
	} else {
		logger.error("Could not register platform. Response was %u!", c);
		return -1;
	}
}

void CMRRestfulConnection::unregisterPlatform(std::vector<std::string> definedIps, LPWSTR agentName)
{
	logger.debug("Unregistering platform...");

	std::wstringstream urlStream;
	urlStream << baseUrl << registrationPath << "/unregisterPlatformIdent";

	json::value ipArray;

	int i = 0;
	for (std::vector<std::string>::iterator it = definedIps.begin(); it != definedIps.end(); it++) {
		std::wstringstream wss;
		wss << it->c_str();
		ipArray[i] = json::value(wss.str());
		i++;
	}

	std::wstringstream argsStream;
	argsStream << "?agentName=" << agentName;

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		response = client.request(methods::POST, argsStream.str().c_str(), ipArray.serialize(), MIME_JSON).get();
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return;
	}

	status_code c = response.status_code();
	if (c == status_codes::OK) {
		auto body = response.extract_string();
		JAVA_INT succ = _wtoi(body.get().c_str());
		
		if (succ < 0) {
			logger.error("Server-side error when unregistering platform! Ignoring...");
		} else {
			logger.debug("Unregistered platform.");
		}
	}
	else {
		logger.error("Could not unregister platform. Response was %u!", c);
	}
}

JAVA_LONG CMRRestfulConnection::registerMethod(JAVA_LONG platformId, LPWSTR className, LPWSTR methodName, LPWSTR returnType, std::vector<LPWSTR> parameterTypes, JAVA_INT modifiers)
{
	logger.debug("Registering method...");

	std::wstringstream urlStream;
	urlStream << baseUrl << registrationPath << "/registerMethodIdent";

	json::value paramArray;

	int i = 0;
	for (std::vector<LPWSTR>::iterator it = parameterTypes.begin(); it != parameterTypes.end(); it++) {
		paramArray[i] = json::value(*it);
		i++;
	}

	std::wstringstream argsStream;
	argsStream << "?platformId=" << platformId << "&className=" << className << "&methodName=" << methodName << "&returnType=" << returnType << "&modifiers=" << modifiers;

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		response = client.request(methods::POST, argsStream.str().c_str(), paramArray.serialize(), MIME_JSON).get();
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return -1;
	}

	status_code c = response.status_code();
	if (c == status_codes::OK) {
		auto body = response.extract_string();
		JAVA_LONG id = _wtoi64(body.get().c_str());
		return id;
	}
	else {
		logger.error("Could not register method. Response was %u!", c);
		return -1;
	}
}

JAVA_LONG CMRRestfulConnection::registerMethodSensorType(JAVA_LONG platformId, LPWSTR agentClassName)
{
	logger.debug("Registering method sensor type...");

	std::wstringstream urlStream;
	urlStream << baseUrl << registrationPath << "/registerMethodSensorTypeIdent";

	std::wstringstream argsStream;
	argsStream << "?platformId=" << platformId << "&fullyQualifiedClassName=" << agentClassName;

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		response = client.request(methods::POST, argsStream.str().c_str(), MIME_JSON).get();
	}
	catch (http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return -1;
	}
	catch (uri_exception const& e) {
		logger.error("uri_exception: %s", e.what());
		return -1;
	}

	status_code c = response.status_code();
	if (c == status_codes::OK) {
		auto body = response.extract_string();
		JAVA_LONG id = _wtoi64(body.get().c_str());
		logger.debug("Method sensor type registered. ID is %lli.", id);
		return id;
	}
	else {
		logger.error("Could not register method sensor type. Response was %u!", c);
		return -1;
	}
}

void CMRRestfulConnection::addSensorTypeToMethod(JAVA_LONG sensorTypeId, JAVA_LONG methodId)
{
	logger.debug("Adding sensor type to method...");

	std::wstringstream urlStream;
	urlStream << baseUrl << registrationPath << "/addSensorTypeToMethod";

	std::wstringstream argsStream;
	argsStream << "?sensorTypeId=" << sensorTypeId << "&methodId=" << methodId;

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		response = client.request(methods::POST, argsStream.str().c_str(), L"", MIME_JSON).get();
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return;
	}

	status_code c = response.status_code();
	if (c != status_codes::OK) {
		logger.error("Could not add sensor type to method. Response was %u!", c);
	}
}

void CMRRestfulConnection::sendDataObjects(std::vector<std::shared_ptr<MethodSensorData>> dataObjects, bool waitForResponse)
{
	logger.debug("Sending data objects...");

	std::wstringstream urlStream;
	urlStream << baseUrl << storagePath << "/addDataObjects";

	json::value objectsJson;
	int i = 0;

	for (auto it = dataObjects.begin(); it != dataObjects.end(); it++) {
		objectsJson[i] = (*it)->toJson();
		i++;
	}

	logger.debug("Json created");

	Logger nestedLogger = logger;

	http_client client(urlStream.str().c_str());
	try {
		auto stream = client.request(methods::POST, L"", objectsJson.serialize(), MIME_JSON).then([&nestedLogger](http_response response)
		{
			status_code c = response.status_code();
			if (c == status_codes::OK) {
				auto body = response.extract_string();
				JAVA_INT succ = _wtoi(body.get().c_str());

				if (succ < 0) {
					nestedLogger.error("Server-side error when sending data objects! Ignoring...");
				}
				else {
					nestedLogger.debug("Data objects sent.");
				}
			}
			else {
				nestedLogger.error("Could not send data objects. Response was %u!", c);
			}
		});

		if (waitForResponse)
		{
			stream.wait();
		}
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
	}
}