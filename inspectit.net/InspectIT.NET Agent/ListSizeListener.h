#pragma once

#include <memory>

namespace inspectit {
	namespace sending {
		namespace strategy {

			class ListSizeListener
			{
			public:
				ListSizeListener() {}
				virtual ~ListSizeListener() {}

				virtual void listSizeChanged(size_t newSize) = 0;
			};

			class ListSizeListenable
			{
				virtual void addListSizeListener(std::shared_ptr<ListSizeListener> listener) = 0;
			};

		}
	}
}