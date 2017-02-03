#include "HelloSensorData.h"



HelloSensorData::HelloSensorData(LPWSTR greeting, JAVA_LONG platformId, JAVA_LONG methodSensorId, JAVA_LONG methodId, JAVA_LONG timestamp)
	: MethodSensorData(platformId, methodSensorId, methodId, timestamp), MeasurementStorage(platformId, methodSensorId, methodId, timestamp)
{
	this->greeting = new WCHAR[MAX_BUFFERSIZE];
	wcscpy_s(this->greeting, MAX_BUFFERSIZE, greeting);
}


HelloSensorData::~HelloSensorData()
{
	delete greeting;
}

LPWSTR HelloSensorData::getJavaTypeName()
{
	return L"HelloData";
}

LPWSTR HelloSensorData::getGreeting()
{
	return greeting;
}

void HelloSensorData::addCount()
{
	counter++;
	wcscat_s(greeting, MAX_BUFFERSIZE, L"I");
}

web::json::value HelloSensorData::toJson()
{
	web::json::value json = super::toJson();
	json[L"greeting"] = web::json::value::string(getGreeting());
	return json;
}

bool HelloSensorData::finished()
{
	return bFinished;
}

std::vector<std::shared_ptr<MethodSensorData>> HelloSensorData::finalizeData()
{
	std::vector<std::shared_ptr<MethodSensorData>> result;
	result.push_back(std::make_shared<HelloSensorData>(greeting, 0, 0, 0));
	return result;
}

int HelloSensorData::getCounter()
{
	return counter;
}

void HelloSensorData::finish()
{
	bFinished = true;
}