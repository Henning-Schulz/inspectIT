#include "CMRRestfulConnection.h"
#include "json_string_prep.h"
#include "ip_calculation.h"

#include <exception>
#include <locale>
#include <codecvt>

using namespace web::http;
using namespace web::http::client;
using namespace web;

using namespace inspectit::connection;
using namespace inspectit::config;
using namespace inspectit::data;
using namespace inspectit::logger;
using namespace inspectit::types;
using namespace inspectit::utils;

CMRConnection* cmrConnection;

namespace inspectit {
	namespace connection {

		bool setupCMRConnection() {
			cmrConnection = new CMRRestfulConnection();

			return ((CMRRestfulConnection*)cmrConnection)->testConnection();
		}

		void shutdownCMRConnection() {
			delete cmrConnection;
		}

	}
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

	connectionTestPath = L"/cmrManagement/testConnection";
	keepAlivePath = L"/keep-alive";
	agentPath = L"/agent";
	storagePath = L"/agent-storage";
	logger.debug("Using CMR URL %ls", baseUrl);
}


CMRRestfulConnection::~CMRRestfulConnection()
{
	logger.debug("Destructor");
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

void CMRRestfulConnection::sendKeepAlive(JAVA_LONG platformId, bool waitForResponse)
{
	logger.debug("Sending keep alive signal...");

	std::wstringstream urlStream;
	urlStream << baseUrl << keepAlivePath << L"/" << platformId;

	logger.debug("Calling URL %ls", urlStream.str().c_str());

	Logger *nestedLogger = &logger;

	http_client client(urlStream.str().c_str());
	try {
		auto stream = client.request(methods::POST, L"", MIME_JSON).then([nestedLogger](http_response response)
		{
			status_code c = response.status_code();
			if (c == status_codes::OK) {
				if (loggerFactory.getLogLevel() == LEVEL_DEBUG) {
					nestedLogger->debug("Response from CMR: %ls", response.extract_string().get().c_str());
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

/*
 * The agentName and the version should not contain whitespaces. Use %20 instead.
 */
std::shared_ptr<AgentConfig> CMRRestfulConnection::registerPlatform(LPWSTR agentName, LPWSTR version)
{
	logger.debug("Registering platform...");

	std::vector<std::string> definedIps = getAllDefinedIPs();

	logger.debug("%i IPs retrieved. First is %s. Last is %s", definedIps.size(), definedIps.front().c_str(), definedIps.back().c_str());

	std::wstringstream urlStream;
	urlStream << baseUrl << agentPath << "/register/" << agentName << "/" << version;

	web::json::value ipArray;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	web::json::value val;

	int i = 0;
	for (std::vector<std::string>::iterator it = definedIps.begin(); it != definedIps.end(); it++) {
		ipArray[i] = web::json::value::string(converter.from_bytes(*it));
		logger.debug("IP %i: %ls", i, ipArray[i].as_string().c_str());
		i++;
	}

	logger.debug("url is: %ls", urlStream.str().c_str());
	logger.debug("ipArray is: %ls", ipArray.serialize().c_str());

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		logger.debug("Sending request...");
		response = client.request(methods::POST, L"", ipArray.serialize().c_str(), MIME_JSON).get();
		logger.debug("Request sent.");
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return std::shared_ptr<AgentConfig>();
	}
	
	status_code c = response.status_code();
	if (c == status_codes::OK) {
		web::json::object obj = response.extract_json().get().as_object();
		logger.debug("Converting JSON to AgentConfig...");
		std::shared_ptr<AgentConfig> agentConfig = std::make_shared<AgentConfig>();
		agentConfig->fromJson(obj);
		logger.debug("Platform registered. ID is %lld", agentConfig->getPlatformId());
		return agentConfig;
	} else {
		logger.error("Could not register platform. Response was %u!", c);
		return std::shared_ptr<AgentConfig>();
	}
}

void CMRRestfulConnection::unregisterPlatform(JAVA_LONG platformId)
{
	logger.debug("Unregistering platform...");

	std::wstringstream urlStream;
	urlStream << baseUrl << agentPath << L"/" << std::to_wstring(platformId) << L"/unregister";

	logger.debug("Calling URL %ls", urlStream.str().c_str());

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		response = client.request(methods::DEL, L"", MIME_JSON).get();
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return;
	}

	status_code c = response.status_code();
	if (c == status_codes::OK) {
		auto body = response.extract_string();
		logger.debug("Response from CMR: %ls", body.get().c_str());
	}
	else {
		logger.error("Could not unregister platform. Response was %u!", c);
	}
}

std::shared_ptr<InstrumentationDefinition> CMRRestfulConnection::analyze(JAVA_LONG platformIdent, std::shared_ptr<Type> type)
{
	logger.debug("Analyzing class %ls for instrumentation...", type->getFqn().c_str());

	std::wstringstream urlStream;
	urlStream << baseUrl << agentPath << "/" << platformIdent << "/analyze";

	http_client client(urlStream.str().c_str());
	http_response response;
	try {
		response = client.request(methods::POST, L"", type->toJson().serialize(), MIME_JSON).get();
	}
	catch (web::http::http_exception) {
		logger.error("Could not connect to %ls", urlStream.str().c_str());
		return std::shared_ptr<InstrumentationDefinition>();
	}

	status_code c = response.status_code();
	if (c == status_codes::OK) {
		auto json = response.extract_json().get();
		if (json.is_null()) {
			// If there is nothing to instrument, the CMR will return null
			return std::shared_ptr<InstrumentationDefinition>();
		}
		else {
			web::json::object obj = json.as_object();
			std::shared_ptr<InstrumentationDefinition> instrumentationDefinition = std::make_shared<InstrumentationDefinition>();
			instrumentationDefinition->fromJson(obj);
			logger.debug("Analysis finished. Received %i method instrumentations.", instrumentationDefinition->getMethodInstrumentationConfigs().size());
			return instrumentationDefinition;
		}
	}
	else {
		logger.error("Could not analyze class %ls. Response was %u!", type->getFqn(), c);
		return std::shared_ptr<InstrumentationDefinition>();
	}
}

void CMRRestfulConnection::instrumentationApplied(JAVA_LONG platformId, MethodSensorMap methodToSensorMap, bool waitForResponse)
{
	logger.debug("Sending instrumentation mapping...");

	std::wstringstream urlStream;
	urlStream << baseUrl << agentPath << L"/" << platformId << L"/instrumentation-applied";

	Logger *nestedLogger = &logger;

	http_client client(urlStream.str().c_str());
	try {
		auto stream = client.request(methods::POST, L"", methodToSensorMap.toJson().serialize(), MIME_JSON).then([nestedLogger](http_response response)
		{
			status_code c = response.status_code();
			if (c == status_codes::OK) {
				if (loggerFactory.getLogLevel() == LEVEL_DEBUG) {
					nestedLogger->debug("Response from CMR: %ls", response.extract_string().get().c_str());
				}
			}
			else {
				nestedLogger->error("Could not send instrumentation mapping. Response was %u!", c);
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

void CMRRestfulConnection::sendDataObjects(std::vector<std::shared_ptr<SensorData>> dataObjects, bool waitForResponse)
{
	logger.debug("Sending data objects...");

	if (dataObjects.size() == 0) {
		logger.debug("No data passed. Doing nothing.");
		return;
	}

	std::shared_ptr<SensorData> last = dataObjects.back();
	JAVA_LONG platformId = last->getPlatformId();
	JAVA_LONG sensorTypeId = last->getSensorTypeId();

	logger.debug("Platform ID is %lli.", platformId);
	logger.debug("Sensor type ID is %lli.", sensorTypeId);

	std::wstringstream urlStream;
	urlStream << baseUrl << storagePath << L"/" << std::to_wstring(platformId) << L"/" << std::to_wstring(sensorTypeId);

	logger.debug("Calling url %ls", urlStream.str().c_str());

	web::json::value objectsJson;
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
				if (loggerFactory.getLogLevel() == LEVEL_DEBUG) {
					nestedLogger->debug("Response from CMR: %ls", response.extract_string().get().c_str());
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
