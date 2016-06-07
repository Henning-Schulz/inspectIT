#pragma once

#include "basehdr.h"

#include <map>

class ShadowStack
{
public:
	ShadowStack();
	~ShadowStack();
};

extern std::map<ThreadID, METHOD_ID> b;