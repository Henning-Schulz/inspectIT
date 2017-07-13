#pragma once

#include <cpprest/json.h>

namespace inspectit {
	namespace json {

		class JsonSendable
		{
		public:
			JsonSendable() {}
			virtual ~JsonSendable() {}

			virtual web::json::value toJson() = 0;
		};

	}
}