#pragma once
#include "Type.h"

#define TYPE_NAME L"Interface"

namespace inspectit {
	namespace types {

		class AnnotationType : Type
		{
		public:
			AnnotationType();
			~AnnotationType();

			std::wstring typeName();
		};

	}
}