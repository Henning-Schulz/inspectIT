#pragma once

#include "StrategyConfig.h"

class SizeBufferStrategyConfig : public StrategyConfig
{
private:
	JAVA_INT size;

public:
	static const LPWSTR CLASS_NAME;

	SizeBufferStrategyConfig();
	~SizeBufferStrategyConfig();

	LPWSTR getClassName();

	JAVA_INT getSize();
	void setSize(JAVA_INT size);
};

