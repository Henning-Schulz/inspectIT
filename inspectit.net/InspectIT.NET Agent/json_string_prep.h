#pragma once

#include "basehdr.h"

#include <string>

namespace inspectit {
	namespace utils {

		std::wstring prepareStringForJson(std::wstring str);

		std::wstring prepareStringForJson(LPWSTR lpwStr);

	}
}