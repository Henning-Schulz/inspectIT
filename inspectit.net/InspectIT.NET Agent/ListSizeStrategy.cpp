#include "ListSizeStrategy.h"



ListSizeStrategy::ListSizeStrategy(size_t maxSize)
{
	this->maxSize = maxSize;
}


ListSizeStrategy::~ListSizeStrategy()
{
}

void ListSizeStrategy::setDataSendingService(DataSendingService *sendingService)
{
	this->sendingService = sendingService;
}

void ListSizeStrategy::start()
{
	active = true;
}

void ListSizeStrategy::stop()
{
	active = false;
}

void ListSizeStrategy::listSizeChanged(size_t newSize)
{
	if (active && newSize > maxSize) {
		sendingService->send();
	}
}