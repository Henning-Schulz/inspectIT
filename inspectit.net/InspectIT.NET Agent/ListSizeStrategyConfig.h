#pragma once

#include "StrategyConfig.h"

class ListSizeStrategyConfig : public StrategyConfig
{
private:
	JAVA_INT listSize = 10;

public:
	const static LPWSTR CLASS_NAME;

	ListSizeStrategyConfig();
	~ListSizeStrategyConfig();

	LPWSTR getClassName();

	JAVA_INT getListSize();
	void setListSize(JAVA_INT listSize);
};
