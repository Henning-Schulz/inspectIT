#pragma once

#include "basehdr.h"

#include <string>

bool patternMatches(LPWSTR pattern, LPWSTR text);
bool patternMatches(LPWSTR pattern, std::wstring text);
bool patternMatches(std::wstring pattern, LPWSTR text);
bool patternMatches(std::wstring pattern, std::wstring text);