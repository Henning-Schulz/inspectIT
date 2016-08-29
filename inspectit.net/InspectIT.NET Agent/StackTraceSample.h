#pragma once

#include "basehdr.h"

#include <vector>
#include <cpprest/json.h>

/* 
 * Represents a stack trace. The offset can be used to indicate
 * that a number of top-level methods is left out.
 * The most low-level method is stored at position 0 in the trace
 * vector while the most top-level method is stored at the end.
 */
class StackTraceSample
{
private:
	std::vector<JAVA_LONG> trace;
	JAVA_INT offset;
	JAVA_LONG timestamp;

public:
	StackTraceSample(std::vector<JAVA_LONG> trace, JAVA_INT offset, JAVA_LONG timestamp = -1);
	~StackTraceSample();

	std::vector<JAVA_LONG> getTrace();
	JAVA_LONG getMethodAt(unsigned int position);
	JAVA_LONG getHighestMethod();
	JAVA_INT getOffset();
	JAVA_LONG getTimestamp();

	web::json::value toJson();
};