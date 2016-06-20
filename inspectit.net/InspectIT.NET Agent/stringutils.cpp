#include "stringutils.h"

#include <vector>

const std::wstring WILDCARD = L"*";

bool patternMatches(LPWSTR pattern, LPWSTR text)
{
	std::wstring wPattern(pattern);
	std::wstring wText(text);
	return patternMatches(wPattern, wText);
}

bool patternMatches(LPWSTR pattern, std::wstring text)
{
	std::wstring wPattern(pattern);
	return patternMatches(wPattern, text);
}

bool patternMatches(std::wstring pattern, LPWSTR text)
{
	std::wstring wText(text);
	return patternMatches(pattern, wText);
}

bool patternMatches(std::wstring pattern, std::wstring text)
{
	std::vector<std::wstring> parts;
	std::wstring::size_type currPos = 0;

	while (currPos < pattern.size()) {
		std::wstring::size_type nextPos = pattern.find(WILDCARD, currPos);

		if (nextPos == std::wstring::npos) {
			nextPos = pattern.size() - 1;
		}

		std::wstring part = pattern.substr(currPos, nextPos - currPos);
		parts.push_back(part);

		currPos = nextPos + 1;
	}

	currPos = 0;

	for (auto it = parts.begin(); it != parts.end(); it++) {
		std::wstring::size_type pos = text.find(*it);

		if (pos == std::wstring::npos) {
			return false;
		} else {
			currPos = pos + it->size();
		}
	}

	return true;
}