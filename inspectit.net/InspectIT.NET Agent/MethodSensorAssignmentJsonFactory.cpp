#include "MethodSensorAssignmentJsonFactory.h"

#include "StackTraceSensorConfig.h"
#include "StackTraceSensorAssignment.h"
#include "TimerSensorConfig.h"



MethodSensorAssignmentJsonFactory::MethodSensorAssignmentJsonFactory()
{
}


MethodSensorAssignmentJsonFactory::~MethodSensorAssignmentJsonFactory()
{
}

std::shared_ptr<MethodSensorAssignment> MethodSensorAssignmentJsonFactory::createFromJson(json::object jsonObject)
{
	utility::string_t configClassName = jsonObject.at(L"sensorConfigClass").as_string();

	logger.debug("Creating MethodSensorAssignment for sensor %ls", configClassName.c_str());

	bool isStackTraceConfig = configClassName.compare(L"rocks.inspectit.shared.cs.ci.sensor.dotNet.DotNetStackTraceSensorConfig") == 0;
	bool isTimerConfig = configClassName.compare(L"rocks.inspectit.shared.cs.ci.sensor.method.impl.TimerSensorConfig") == 0;

	if (isStackTraceConfig || isTimerConfig) {
		std::shared_ptr<MethodSensorAssignment> assignment;

		auto it = jsonObject.find(L"baseMethodAssignment");

		if (it != jsonObject.end()) {
			json::value baseMethodAssignmentVal = it->second;

			std::shared_ptr<StackTraceSensorAssignment> stAssignment = std::make_shared<StackTraceSensorAssignment>();

			json::object baseMethodAssignmentJson = baseMethodAssignmentVal.as_object();
			std::shared_ptr<MethodSensorAssignment> baseMethodAssignment = createFromJson(baseMethodAssignmentJson);

			stAssignment->setBaseMethodAssignment(baseMethodAssignment);
			assignment = stAssignment;
		}
		else {
			assignment = std::make_shared<MethodSensorAssignment>();
		}

		if (isStackTraceConfig) {
			assignment->setSensorClassName(StackTraceSensorConfig::CLASS_NAME);
		}
		else {
			assignment->setSensorClassName(TimerSensorConfig::CLASS_NAME);
		}
		utility::string_t className = jsonObject.at(L"className").as_string();
		assignment->setClassName(className);
		bool superclass = jsonObject.at(L"superclass").as_bool();
		assignment->setSuperclass(superclass);
		bool interf = jsonObject.at(L"interf").as_bool();
		assignment->setInterface(interf);
		utility::string_t methodName = jsonObject.at(L"methodName").as_string();
		assignment->setMethodName(methodName);
		json::value paramVal = jsonObject.at(L"parameters");
		std::vector<std::wstring> params;
		if (!paramVal.is_null()) {
			json::array parameters = paramVal.as_array();
			for (auto it = parameters.begin(); it != parameters.end(); it++) {
				utility::string_t param = it->as_string();
				params.push_back(param);
			}

			assignment->setParameters(params);
			assignment->setParametersSpecified(true);
		} else {
			assignment->setParametersSpecified(false);
		}
		
		bool constructor = jsonObject.at(L"constructor").as_bool();
		assignment->setConstructor(constructor);
		bool publicModifier = jsonObject.at(L"publicModifier").as_bool();
		assignment->setPublicModifier(publicModifier);
		bool protectedModifier = jsonObject.at(L"protectedModifier").as_bool();
		assignment->setProtectedModifier(protectedModifier);
		bool privateModifier = jsonObject.at(L"privateModifier").as_bool();
		assignment->setPrivateModifier(privateModifier);
		bool defaultModifier = jsonObject.at(L"defaultModifier").as_bool();
		assignment->setDefaultModifier(defaultModifier);

		return assignment;
	}
	else {
		logger.warn("Cannot instantiate MethodSensorAssignment for sensor %ls", configClassName.c_str());
		return std::shared_ptr<MethodSensorAssignment>();
	}
}