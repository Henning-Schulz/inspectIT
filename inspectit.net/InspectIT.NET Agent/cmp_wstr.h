#pragma once

#include "basehdr.h"

struct cmp_wstr
{
	bool operator()(wchar_t const *a, wchar_t const *b) const
	{
		return wcscmp(a, b) < 0;
	}
};