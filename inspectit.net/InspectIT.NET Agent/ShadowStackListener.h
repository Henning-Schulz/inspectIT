#pragma once

class ShadowStackListener {
public:
	ShadowStackListener() {}
	virtual ~ShadowStackListener() {}

	virtual void stackSizeChanged(size_t newSize) = 0;
	virtual void stackSizeWillChange(size_t newSize) = 0;
};