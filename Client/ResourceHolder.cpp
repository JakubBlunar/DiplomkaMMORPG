#include "ResourceHolder.h"
#include <cassert>

template <typename Resource>
ResourceHolder<Resource>::ResourceHolder()
{
}

template <typename Resource>
void ResourceHolder<Resource>::load(const std::string& filename)
{
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	auto inserted = mResourceMap.insert(std::make_pair(filename, std::move(resource)));
	assert(inserted.second);
}

template <typename Resource>
Resource& ResourceHolder<Resource>::get(const std::string& filename)
{
	auto found = mResourceMap.find(filename);
	if(found != mResourceMap.end())
	{
		load(filename);
		found = mResourceMap.find(filename);
	}
	return *found->second;
}