#pragma once

namespace inspectit {
	namespace sending {

		class IntegerIterable {
		public:
			IntegerIterable() {}
			virtual ~IntegerIterable() {}

			virtual int getNextInteger(int startPosition) = 0;
		};

	}
}