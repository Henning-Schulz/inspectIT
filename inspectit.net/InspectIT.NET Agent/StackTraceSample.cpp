#include "StackTraceSample.h"

StackTraceSample::StackTraceSample(std::vector<JAVA_LONG> trace, JAVA_INT offset, JAVA_LONG timestamp)
{
	this->trace = trace;
	this->offset = offset;
	this->timestamp = timestamp;
}

StackTraceSample::~StackTraceSample()
{
}

std::vector<JAVA_LONG> StackTraceSample::getTrace()
{
	return trace;
}

JAVA_LONG StackTraceSample::getMethodAt(unsigned int position)
{
	if (position < trace.size()) {
		return trace[position];
	}
	else {
		return -1;
	}
}

JAVA_LONG StackTraceSample::getHighestMethod()
{
	if (trace.empty() || offset > 0) {
		return -1;
	}
	else {
		return trace.back();
	}
}

JAVA_INT StackTraceSample::getOffset()
{
	return offset;
}

web::json::value StackTraceSample::toJson()
{
	web::json::value json;
	json[L"offset"] = web::json::value::number(offset);

	web::json::value traceJson;
	int i = 0;
	for (auto it = trace.begin(); it != trace.end(); it++) {
		traceJson[i] = web::json::value::number(*it);
		i++;
	}

	json[L"trace"] = traceJson;

	return json;
}