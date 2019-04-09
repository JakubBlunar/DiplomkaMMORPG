#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <set>
#include <ctime>

class Utils
{
public:
	Utils();
	~Utils();

	static std::vector<std::string> splitPath(std::string path, std::set<char> delimiters);
	static std::string removeExtension(const std::string& filename);

	static time_t getActualUtcTime();
	static time_t utcTimeToLocalTime(time_t utcTime);
};

#endif
