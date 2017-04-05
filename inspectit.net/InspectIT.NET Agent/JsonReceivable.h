#pragma once

#include <cpprest/json.h>

using namespace web;

class JsonReceivable {

public:
	JsonReceivable() {}
	virtual ~JsonReceivable() {}

	virtual void fromJson(json::object json) = 0;

};