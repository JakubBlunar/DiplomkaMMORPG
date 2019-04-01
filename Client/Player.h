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
#include <unordered_set>

class GameEvent;
using json = nlohmann::json;

class Player :
	public Entity {
public:
	Player(bool playerControlled);
	~Player();

	Entity* target;
	Map* map;

	void handleEvent(GameEvent* event) override;
	void update(sf::Time elapsedTime, Map* map, Game* g) override;

	EntityType getType() override;
	EntityCategory getEntityCategory() override;
	uint16 getCollisionMask() override;

	

	void updateMovementAnimation();

	void loadFromJson(json jsonData);


	b2Fixture* getRange() const;
	void setRange(b2Fixture* fixture);

	std::unordered_set<Entity*> rangeEntities;
	bool isControlledByPlayer() const;

	void addRangeEntity(Entity* entity, Game* g);
	void removeRangeEntity(Entity* entity);

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

	void addSpell(SpellInfo* si) {
		spells.push_back(si);
	}



	SpellCooldown* getGlobalCooldown(Game* g) const;
	void setGlobalCooldown(SpellCooldown* cooldown);

	void setCooldown(int spellType, SpellCooldown* cooldown);
	SpellCooldown* getCooldown(int spellType, Game* g) const;

	bool isHostile(Entity* entity) const;

	void setMap(Map* map) {
		this->map = map;
	}
private:
	AttributesComponent* attributesComponent;

	bool playerControlled;
	RenderComponent* renderComponent;

	b2Fixture* rangeView;

	sf::Vector2f lastMovement;
	sf::Time sendingTime;

	sf::Time lastSendedMovementTime;

	SpellCooldown* globalCooldown;
	std::map<int, SpellCooldown*> spellCooldowns;

	void sendPosition(Game* g) const;

	std::vector<SpellInfo*> spells;
};

#endif
