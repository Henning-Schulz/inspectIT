#pragma once

#include "IntegerIterable.h"

namespace inspectit {
	namespace sending {

		class IntegerIterator
		{
		private:
			int current;
			int nextStart;
			int max;
			int tmp;
			bool hasNextCalled;

			IntegerIterable *iterated;

		public:
			IntegerIterator(IntegerIterable *iterated, int start, int max);
			~IntegerIterator();

			bool hasNext();
			int next();
		};

	}
}