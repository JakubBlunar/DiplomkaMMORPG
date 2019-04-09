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

std::string Utils::removeExtension(const std::string& filename) {
	size_t lastDot = filename.find_last_of('.');
	if (lastDot == std::string::npos) return filename;
	return filename.substr(0, lastDot);
}

time_t Utils::getActualUtcTime() {
	time_t now = time(nullptr);
	struct tm timeInfo;
	gmtime_s(&timeInfo, &now);
	return (time_t)difftime(mktime(&timeInfo), 0);
}

time_t Utils::utcTimeToLocalTime(time_t utcTime) {
	time_t now = time(nullptr);
	struct tm timeInfo;
	gmtime_s(&timeInfo, &now);
	time_t utcTimeNow = (time_t)difftime(mktime(&timeInfo), 0);
	time_t timeZone = now - utcTimeNow;
	return utcTime + timeZone;
}
