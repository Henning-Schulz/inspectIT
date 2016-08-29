#include "StackTraceSample.h"

StackTraceSample::StackTraceSample(std::vector<JAVA_LONG> trace, JAVA_INT offset, JAVA_LONG timestamp)
{
	this->trace = trace;
	this->offset = offset;

	if (timestamp < 0) {
		this->timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	} else {
		this->timestamp = timestamp;
	}
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

JAVA_LONG StackTraceSample::getTimestamp()
{
	return this->timestamp;
}

web::json::value StackTraceSample::toJson()
{
	web::json::value json;
	json[L"offset"] = web::json::value::number(offset);
	json[L"timestamp"] = web::json::value::number(timestamp);

	web::json::value traceJson;
	int i = 0;
	for (auto it = trace.begin(); it != trace.end(); it++) {
		traceJson[i] = web::json::value::number(*it);
		i++;
	}

	json[L"trace"] = traceJson;

	return json;
}