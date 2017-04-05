#pragma once
#include "Type.h"

#define TYPE_NAME L"Interface"

class AnnotationType : Type
{
public:
	AnnotationType();
	~AnnotationType();

	std::wstring typeName();
};

