#include "stdafx.h"
#include "Utils.h"

Utils::Utils() {}


Utils::~Utils() {}

std::vector<std::string> Utils::splitPath(std::string path, std::set<char> delimiters) {

	std::vector<std::string> result;
	char const* pch = path.c_str();
	char const* start = pch;
	for (; *pch; ++pch) {
		if (delimiters.find(*pch) != delimiters.end()) {
			if (start != pch) {
				std::string str(start, pch);
				result.push_back(str);
			}
			else {
				result.push_back("");
			}
			start = pch + 1;
		}
	}
	result.push_back(start);

	return result;
}

std::string Utils::removeExtension(const std::string & filename)
{
    size_t lastdot = filename.find_last_of('.');
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot);
}
