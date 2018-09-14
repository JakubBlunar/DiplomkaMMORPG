#include "MapContactListener.h"
#include "Entity.h"
#include "Player.h"

void MapContactListener::BeginContact(b2Contact * contact)
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
		if (type == EntityType::PLAYER) {
			Player* p = (Player*) entity;
			if(p->getMelleRange() == first) {
				p->melleRangeEntities.push_back(collideWith);
			}
			if(p->getMelleView() == first) {
				p->melleViewEntities.push_back(collideWith);
			}
		}
	}

	if (secondIsSensor) {
		Entity* entity = (Entity*)second->GetBody()->GetUserData();
		Entity* collideWith = (Entity*) first->GetBody()->GetUserData();
		EntityType type = entity->getType();
		if (type == EntityType::PLAYER) {
			Player* p = (Player*) entity;
			if(p->getMelleRange() == second) {
				p->melleRangeEntities.push_back(collideWith);
			}
			if(p->getMelleView() == second) {
				p->melleViewEntities.push_back(collideWith);
			}
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
		if (type == EntityType::PLAYER) {
			Player* p = (Player*) entity;
			if(p->getMelleRange() == first) {
				p->melleRangeEntities.erase(std::find(p->melleRangeEntities.begin(), p->melleRangeEntities.end(), collideWith));
			}
			if(p->getMelleView() == first) {
				p->melleViewEntities.erase(std::find(p->melleViewEntities.begin(), p->melleViewEntities.end(), collideWith));
			}
		}
	}

	if (secondIsSensor) {
		Entity* entity = (Entity*)second->GetBody()->GetUserData();
		Entity* collideWith = (Entity*) first->GetBody()->GetUserData();
		EntityType type = entity->getType();
		if (type == EntityType::PLAYER) {
			Player* p = (Player*) entity;
			if(p->getMelleRange() == second) {
				p->melleRangeEntities.erase(std::find(p->melleRangeEntities.begin(), p->melleRangeEntities.end(), collideWith));
			}
			if(p->getMelleView() == second) {
				p->melleViewEntities.erase(std::find(p->melleViewEntities.begin(), p->melleViewEntities.end(), collideWith));
			}
		}
	}
}
