#include "json_string_prep.h"

#include <vector>

std::vector<std::wstring> charsToRep = { L" ", L"`", L"<", L">" };
std::vector<std::wstring> replacements = { L"%20", L"%60", L"%3C", L"%3E" };


std::wstring prepareStringForJson(std::wstring str)
{
	for (int j = 0; j < charsToRep.size(); j++) {
		size_t idx = str.find(charsToRep.at(j));
		if (idx >= 0 && idx < str.size()) {
			str.replace(idx, 1, replacements.at(j));
		}
	}

	return str;
}

std::wstring prepareStringForJson(LPWSTR lpwStr)
{
	return prepareStringForJson(std::wstring(lpwStr));
}