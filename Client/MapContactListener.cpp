#include "MapContactListener.h"
#include "Entity.h"
#include "Player.h"
#include "Map.h"

void MapContactListener::BeginContact(b2Contact * contact)
{
	b2Fixture* first = contact->GetFixtureA();
	b2Fixture* second = contact->GetFixtureB();

	if (!first || !second) {
		return;
	}

	bool firstIsSensor = first->IsSensor();
	bool secondIsSensor = second->IsSensor();

	if(firstIsSensor && secondIsSensor) {
		return;
	}

	if (firstIsSensor) {
		Entity* entity = (Entity*)first->GetBody()->GetUserData();
		Entity* collideWith = (Entity*) second->GetBody()->GetUserData();
		EntityType type = entity->getType();
		if (type == EntityType::PLAYER && collideWith->getType() == EntityType::NPC) {
			Player* p = (Player*) entity;
			p->addRangeEntity(collideWith, map->getGame());
		}
		return;
	}

	if (secondIsSensor) {
		Entity* entity = (Entity*)second->GetBody()->GetUserData();
		Entity* collideWith = (Entity*) first->GetBody()->GetUserData();
		EntityType type = entity->getType();
		if (type == EntityType::PLAYER && collideWith->getType() == EntityType::NPC) {
			Player* p = (Player*) entity;
			p->addRangeEntity(collideWith, map->getGame());
		}
		return;
	}

	Entity* entity = (Entity*)first->GetBody()->GetUserData();
	Entity* entity2 = (Entity*)second->GetBody()->GetUserData();
	if (entity->getType() == EntityType::SPELL) {
		Spell* spell = (Spell*) entity;
		if (spell->getTarget() == entity2) {
			map->removeSpell(spell);
			return;
		}
	}
	if(entity2->getType() == EntityType::SPELL) {
		Spell* spell = (Spell*) entity2;
		if (spell->getTarget() == entity) {
			map->removeSpell(spell);
		}
	}

	
}

void MapContactListener::EndContact(b2Contact * contact)
{
	b2Fixture* first = contact->GetFixtureA();
	b2Fixture* second = contact->GetFixtureB();


	bool firstIsSensor = first->IsSensor();
	bool secondIsSensor = second->IsSensor();

	if(firstIsSensor && secondIsSensor) {
		return;
	}

	if (firstIsSensor) {
		Entity* entity = (Entity*)first->GetBody()->GetUserData();
		Entity* collideWith = (Entity*) second->GetBody()->GetUserData();
		EntityType type = entity->getType();
		if (type == EntityType::PLAYER && collideWith->getType() == EntityType::NPC) {
			Player* p = (Player*) entity;
			p->removeRangeEntity(collideWith);
		}
	}

	if (secondIsSensor) {
		Entity* entity = (Entity*)second->GetBody()->GetUserData();
		Entity* collideWith = (Entity*) first->GetBody()->GetUserData();
		EntityType type = entity->getType();
		if (type == EntityType::PLAYER && collideWith->getType() == EntityType::NPC) {
			Player* p = (Player*) entity;
			p->removeRangeEntity(collideWith);
		}
	}
}
