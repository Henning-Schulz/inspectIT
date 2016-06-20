#include "ListSizeStrategyConfig.h"



const LPWSTR ListSizeStrategyConfig::CLASS_NAME = L"rocks.inspectit.agent.java.sending.impl.ListSizeStrategy";

ListSizeStrategyConfig::ListSizeStrategyConfig()
{
}


ListSizeStrategyConfig::~ListSizeStrategyConfig()
{
}

LPWSTR ListSizeStrategyConfig::getClassName()
{
	return CLASS_NAME;
}

JAVA_INT ListSizeStrategyConfig::getListSize()
{
	return listSize;
}

void ListSizeStrategyConfig::setListSize(JAVA_INT listSize)
{
	this->listSize = listSize;
}