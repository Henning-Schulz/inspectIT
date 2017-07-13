#pragma once

#include <cpprest/json.h>

namespace inspectit {
	namespace json {

		class JsonReceivable {

		public:
			JsonReceivable() {}
			virtual ~JsonReceivable() {}

			virtual void fromJson(web::json::object json) = 0;

		};

	}
}