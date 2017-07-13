#pragma once
#include "basehdr.h"
#include "JsonReceivable.h"
#include "Logger.h"

#include <map>

namespace inspectit {
	namespace config {

		class AbstractSensorTypeConfig :
			public inspectit::json::JsonReceivable
		{
		private:
			JAVA_LONG id;
			std::wstring className;

			std::map<std::wstring, std::wstring> parameters;

			inspectit::logger::Logger logger = loggerFactory.createLogger("AbstractSensorTypeConfig");

		public:
			AbstractSensorTypeConfig();
			~AbstractSensorTypeConfig();

			JAVA_LONG getId();
			std::wstring getClassName();

			std::map<std::wstring, std::wstring> getParameters();

			void fromJson(web::json::object json);
		};

	}
}