#include "MapsManager.h"
#include "Map.h"


s::MapsManager::MapsManager()
{

}


s::MapsManager::~MapsManager()
{
}

s::Map* s::MapsManager::getMap(int id)
{
	auto it = maps.find(id);
	if (it != maps.end())
	{
		return it->second;
	}
	return nullptr;
}

void s::MapsManager::update(sf::Time elapsedTime, s::Server * s)
{
	for (std::map<int, s::Map*>::iterator it = maps.begin(); it != maps.end(); ++it)
	{
		it->second->update(elapsedTime, s);
	}
}
