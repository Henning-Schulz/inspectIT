#pragma once

#include "ListSizeListener.h"
#include "DataSendingService.h"

namespace inspectit {
	namespace sending {
		namespace strategy {

			class ListSizeStrategy : public SendingStrategy, public ListSizeListener
			{
			private:
				DataSendingService *sendingService;
				size_t maxSize;
				bool active = false;

			public:
				ListSizeStrategy(size_t maxSize);
				~ListSizeStrategy();

				void setDataSendingService(DataSendingService *sendingService);

				void start();
				void stop();

				void listSizeChanged(size_t newSize);
			};

		}
	}
}