#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "PositionComponent.h"
#include "../Client/json.hpp"
#include "RenderComponent.h"
#include <list>
#include "AttributesComponent.h"
#include "Spell.h"
#include <map>

class GameEvent;
using json = nlohmann::json;

class Player :
	public Entity {
public:
	Player(bool playerControlled);
	~Player();

	Entity* target;

	void handleEvent(GameEvent* event) override;
	void update(sf::Time elapsedTime, Map* map, Game* g) override;

	EntityType getType() override;
	EntityCategory getEntityCategory() override;
	uint16 getCollisionMask() override;

	

	void updateMovementAnimation();

	void loadFromJson(json jsonData);

	b2Fixture* getMelleView() const;
	void setMelleView(b2Fixture* fixture);

	b2Fixture* getMelleRange() const;
	void setMelleRange(b2Fixture* fixture);

	std::list<Entity*> melleRangeEntities;
	std::list<Entity*> melleViewEntities;
	bool isControlledByPlayer() const;

	void setMovementDirection(sf::Vector2f direction, Game* g);

	PositionComponent* positionComponent;
	MovementData lastServerPosition;

	AttributesComponent* getAttributesComponent() const;

	SpellInfo* castingSpell;
	sf::Time startCastTime;

	void setCastingSpell(SpellInfo* spell) {
		this->castingSpell = spell;
	}

	void setStartCastTime(sf::Time startTime) {
		this->startCastTime = startTime;
	}

	SpellInfo* getCastingSpell() const {
		return castingSpell;
	}

	sf::Time getStartCastingTime() const {
		return startCastTime;
	}

	std::vector<SpellInfo*>* getSpells() {
		return &spells;
	}

	void castSpell(SpellInfo* spellInfo, Map* map, Game* g);

	void setTarget(Entity* target) {
		this->target = target;
	}
	Entity* getTarget() const {
		return target;
	}



	SpellCooldown* getGlobalCooldown(Game* g) const;
	void setGlobalCooldown(SpellCooldown* cooldown);

	void setCooldown(int spellType, SpellCooldown* cooldown);
	SpellCooldown* getCooldown(int spellType, Game* g) const;

private:
	AttributesComponent* attributesComponent;

	bool playerControlled;
	RenderComponent* renderComponent;

	b2Fixture* melleView;
	b2Fixture* melleRange;

	sf::Vector2f lastMovement;
	sf::Time sendingTime;

	sf::Time lastSendedMovementTime;

	SpellCooldown* globalCooldown;
	std::map<int, SpellCooldown*> spellCooldowns;


	void sendPosition(Game* g) const;

	std::vector<SpellInfo*> spells;
};

#endif
