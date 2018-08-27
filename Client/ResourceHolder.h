#ifndef RESOURCE_HOLDER_H
#define RESOURCE_HOLDER_H

#include <string>
#include <memory>
#include <map>
#include <cassert>

template<typename Resource>
class ResourceHolder {

public:
	void load(const std::string& filename);
	Resource& get(const std::string& filename);

	ResourceHolder(const ResourceHolder&) = delete;
	ResourceHolder& operator=(const ResourceHolder) = delete;

	static ResourceHolder* instance() {
		static ResourceHolder instance;
		return &instance;
	}

private:
	std::map<std::string, std::unique_ptr<Resource>> mResourceMap;
	ResourceHolder();
};

template<typename Resource>
ResourceHolder<Resource>::ResourceHolder() {}

template<typename Resource>
void ResourceHolder<Resource>::load(const std::string& filename) {
	std::unique_ptr<Resource> resource(new Resource());

	if (!resource->loadFromFile("Data/" + filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	auto inserted = mResourceMap.insert(std::make_pair(filename, std::move(resource)));
	//assert(inserted.second);
}

template<typename Resource>
Resource& ResourceHolder<Resource>::get(const std::string& filename) {
	auto found = mResourceMap.find(filename);
	if (found == mResourceMap.end()) {
		load(filename);
		found = mResourceMap.find(filename);
	}
	return *found->second;
}

#endif
