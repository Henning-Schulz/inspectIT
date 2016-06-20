#pragma once

#include "basehdr.h"

class ShadowStackHolder
{
public:
	ShadowStackHolder() {}
	virtual ~ShadowStackHolder() {}

	virtual void pushMethod(ThreadID threadId, METHOD_ID methodId) = 0;
	virtual void popMethod(ThreadID threadId) = 0;
};