#ifndef RESOURCE_HOLDER_H
#define RESOURCE_HOLDER_H

#include <string>
#include <memory>
#include <map>

template <typename Resource>
class ResourceHolder
{
public:
	ResourceHolder();
	void load(const std::string& filename);
	Resource& get(const std::string& filename);
private:
	std::map<std::string, std::unique_ptr<Resource>> mResourceMap;
};

#endif
