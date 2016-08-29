#include "CMRRestfulConnection.h"
#include "json_string_prep.h"

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
	char *url;
	size_t len;
	errno_t err = _dupenv_s(&url, &len, "URL_CMR");
	if (err || len == 0) {
		logger.warn("No URL to the CMR specified. Please use the environment variable URL_CMR. Trying to connect to http://localhost:8182");
		baseUrl = L"http://localhost:8182/rest";
	}
	else {
		std::wstringstream urlStream;
		std::string sUrl(url);
		std::wstring wUrl(sUrl.begin(), sUrl.end());
		std::wstring prefix(L"http://");

		if (wUrl.compare(0, prefix.size(), prefix) != 0) {
			// url does not start with http://
			urlStream << prefix;
		}

		urlStream << wUrl << L"/rest";

		baseUrl = urlStream.str();
	}

	registrationPath = L"/dotNetRegistration";
	connectionTestPath = L"/dotNetConnection/test";
	configPath = L"/dotNetAgentConfig";
	storagePath = L"/dotNetAgentStorage";
	logger.debug("Using CMR URL %ls", baseUrl);
}


CMRRestfulConnection::~CMRRestfulConnection()
{
	logger.info("Destructor");
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
	argsStream << "?agentName=" << prepareStringForJson(agentName) << "&version=" << version;

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
		logger.debug("Platform registered. ID is %lld", id);
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
	argsStream << "?agentName=" << prepareStringForJson(agentName);

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
	argsStream << "?platformId=" << platformId << "&className=" << prepareStringForJson(className)
		<< "&methodName=" << methodName << "&returnType=" << prepareStringForJson(returnType) << "&modifiers=" << modifiers;

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		logger.debug("Sending request with arguments %ls", argsStream.str().c_str());
		response = client.request(methods::POST, argsStream.str().c_str(), paramArray.serialize(), MIME_JSON).get();
	}
	catch (web::http::http_exception) {
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
		logger.debug("Successfully registered method. ID is %lld.", id);
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
		logger.debug("Method sensor type registered. ID is %lld.", id);
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

std::shared_ptr<StrategyConfig> CMRRestfulConnection::getStrategyConfig(JAVA_LONG platformId, const wchar_t* path)
{
	std::wstringstream argsStream;
	argsStream << "?platformId=" << platformId;

	http_client client(path);
	http_response response;
	try {
		response = client.request(methods::GET, argsStream.str().c_str()).get();
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", path);
		return nullptr;
	}

	status_code c = response.status_code();
	if (c == status_codes::OK) {
		json::object obj = response.extract_json().get().as_object();
		return strategyConfigFactory.createFromJson(obj);
	}
	else {
		logger.error("Could not get strategy config. Response was %u!", c);
		return nullptr;
	}
}

std::shared_ptr<StrategyConfig> CMRRestfulConnection::getSendingStrategyConfig(JAVA_LONG platformId)
{
	std::wstringstream urlStream;
	urlStream << baseUrl << configPath << "/getSendingStrategyConfig";

	return getStrategyConfig(platformId, urlStream.str().c_str());
}

std::shared_ptr<StrategyConfig> CMRRestfulConnection::getBufferStrategyConfig(JAVA_LONG platformId)
{
	std::wstringstream urlStream;
	urlStream << baseUrl << configPath << "/getBufferStrategyConfig";

	return getStrategyConfig(platformId, urlStream.str().c_str());
}

std::vector<std::shared_ptr<MethodSensorConfig>> CMRRestfulConnection::getMethodSensorConfigs(JAVA_LONG platformId)
{
	std::wstringstream urlStream;
	urlStream << baseUrl << configPath << "/getMethodSensorConfigs";

	std::wstringstream argsStream;
	argsStream << "?platformId=" << platformId;

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		response = client.request(methods::GET, argsStream.str().c_str()).get();
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return std::vector<std::shared_ptr<MethodSensorConfig>>();
	}

	status_code c = response.status_code();
	if (c == status_codes::OK) {
		json::array arr = response.extract_json().get().as_array();
		std::vector<std::shared_ptr<MethodSensorConfig>> sensorConfigs;
		
		for (auto it = arr.begin(); it != arr.end(); it++) {
			json::object obj = it->as_object();
			logger.debug("Creating object from Json...");
			std::shared_ptr<MethodSensorConfig> config = sensorConfigFactory.createFromJson(obj);
			logger.debug("Object created.");

			if (config) {
				logger.debug("Adding config...");
				sensorConfigs.push_back(config);
				logger.debug("Config added.");
			}
			else {
				logger.warn("Could not create object from Json");
			}
		}
		
		return sensorConfigs;
	}
	else {
		logger.error("Could not get method sensor config. Response was %u!", c);
		return std::vector<std::shared_ptr<MethodSensorConfig>>();
	}
}

std::vector<std::shared_ptr<MethodSensorAssignment>> CMRRestfulConnection::getMethodSensorAssignments(JAVA_LONG platformId)
{
	std::wstringstream urlStream;
	urlStream << baseUrl << configPath << "/getSensorAssignments";

	std::wstringstream argsStream;
	argsStream << "?platformId=" << platformId;

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		response = client.request(methods::GET, argsStream.str().c_str()).get();
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return std::vector<std::shared_ptr<MethodSensorAssignment>>();
	}

	status_code c = response.status_code();
	if (c == status_codes::OK) {
		json::array arr = response.extract_json().get().as_array();
		std::vector<std::shared_ptr<MethodSensorAssignment>> sensorAssignments;

		for (auto it = arr.begin(); it != arr.end(); it++) {
			json::object obj = it->as_object();
			logger.debug("Creating object from Json...");
			std::shared_ptr<MethodSensorAssignment> ass = sensorAssignmentFactory.createFromJson(obj);
			logger.debug("Object created.");

			if (ass) {
				logger.debug("Adding config...");
				sensorAssignments.push_back(ass);
				logger.debug("Config added.");
			}
			else {
				logger.warn("Could not create object from Json");
			}
		}

		return sensorAssignments;
	}
	else {
		logger.error("Could not get method sensor assignment. Response was %u!", c);
		return std::vector<std::shared_ptr<MethodSensorAssignment>>();
	}
}

std::vector<std::wstring> CMRRestfulConnection::getExcludedClasses(JAVA_LONG platformId)
{
	std::wstringstream urlStream;
	urlStream << baseUrl << configPath << "/getExcludedClasses";

	std::wstringstream argsStream;
	argsStream << "?platformId=" << platformId;

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		response = client.request(methods::GET, argsStream.str().c_str()).get();
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return std::vector<std::wstring>();
	}

	status_code c = response.status_code();
	if (c == status_codes::OK) {
		json::array arr = response.extract_json().get().as_array();
		std::vector<std::wstring> excludedClasses;

		for (auto it = arr.begin(); it != arr.end(); it++) {
			excludedClasses.push_back(it->as_string());
		}

		return excludedClasses;
	}
	else {
		logger.error("Could not get method sensor assignment. Response was %u!", c);
		return std::vector<std::wstring>();
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

	Logger *nestedLogger = &logger;

	http_client client(urlStream.str().c_str());
	try {
		auto stream = client.request(methods::POST, L"", objectsJson.serialize(), MIME_JSON).then([nestedLogger](http_response response)
		{
			status_code c = response.status_code();
			if (c == status_codes::OK) {
				auto body = response.extract_string();
				JAVA_INT succ = _wtoi(body.get().c_str());

				if (succ < 0) {
					nestedLogger->error("Server-side error when sending data objects! Ignoring...");
				}
				else {
					nestedLogger->debug("Data objects sent.");
				}
			}
			else {
				nestedLogger->error("Could not send data objects. Response was %u!", c);
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

void CMRRestfulConnection::sendKeepAlive(JAVA_LONG platformId, bool waitForResponse)
{
	logger.debug("Sending keep alive signal...");

	std::wstringstream urlStream;
	urlStream << baseUrl << storagePath << "/keepAlive";

	std::wstringstream argsStream;
	argsStream << "?platformId=" << platformId;

	Logger *nestedLogger = &logger;

	http_client client(urlStream.str().c_str());
	try {
		auto stream = client.request(methods::POST, argsStream.str().c_str(), MIME_JSON).then([nestedLogger](http_response response)
		{
			status_code c = response.status_code();
			if (c == status_codes::OK) {
				auto body = response.extract_string();
				JAVA_INT succ = _wtoi(body.get().c_str());

				if (succ < 0) {
					nestedLogger->error("Server-side error when sending keep alive signal! Ignoring...");
				}
				else {
					nestedLogger->debug("Keep alive signal sent.");
				}
			}
			else {
				nestedLogger->error("Could not send keep alive signal. Response was %u!", c);
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