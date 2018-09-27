#include "Spawn.h"



s::Spawn::Spawn()
{
}


s::Spawn::~Spawn()
{
}

void s::Spawn::init()
{
	spawnedNpcs.clear();
	spawnedNpcs.reserve(maxSpawnedNpcs);
}

void s::Spawn::update(sf::Time elapsedTime, s::Server* s) {
	lastUpdate += elapsedTime;




}
