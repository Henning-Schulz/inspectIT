#include "HelloSensor.h"

#include "DataSendingService.h"
#include "HelloSensorData.h"

#include <string>
#include <memory>

int counter = 0;

HelloSensor::HelloSensor()
{
}


HelloSensor::~HelloSensor()
{
}

void HelloSensor::beforeBody(METHOD_ID methodID)
{
	std::wstringstream wss;
	wss << "Hi from before " << methodID;
	std::shared_ptr<HelloSensorData> data = std::make_shared<HelloSensorData>((LPWSTR)wss.str().c_str(), getPlatformId(), getSensorTypeId(), methodID);
	dataSendingService->addMethodSensorData(data, std::to_string(counter));
	counter++;
}

void HelloSensor::afterBody(METHOD_ID methodID)
{
	std::shared_ptr<HelloSensorData> data = std::static_pointer_cast<HelloSensorData>(dataSendingService->getMeasurementStorage(getSensorTypeId(), methodID, "after"));

	if (data == nullptr || data->finished()) {
		std::wstringstream wss;
		wss << "Hi from after " << methodID << " ";
		std::shared_ptr<HelloSensorData> data = std::make_shared<HelloSensorData>((LPWSTR)wss.str().c_str(), getPlatformId(), getSensorTypeId(), methodID);
		dataSendingService->addMeasurementStorage(data, "after");
	}
	else {
		data->addCount();
		if (data->getCounter() >= 5) {
			data->finish();
			logger.debug("Data finished");
		}
	}
}