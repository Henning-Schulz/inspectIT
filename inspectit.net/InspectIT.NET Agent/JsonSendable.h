#pragma once

#include <cpprest/json.h>

class JsonSendable
{
public:
	JsonSendable() {}
	virtual ~JsonSendable() {}

	virtual web::json::value toJson() = 0;
};