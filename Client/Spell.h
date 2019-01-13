#ifndef SPELL_H
#define SPELL_H

#include "Entity.h"
#include "PositionComponent.h"
#include "RenderComponent.h"
#include "RenderSprite.h"
#include "EventNpcChanges.h"
#include "json.hpp"

struct SpellInfo {
	std::string name;
	float manaCost;
	sf::Time cooldownTime;
	sf::Time globalCooldownTime;
	sf::Time castingTime;
	int id;
	RenderSprite icon;
	float maxRange;
	std::string description;
	bool hasEntity;
};

class Spell: public Entity {
	PositionComponent* positionComponent;
	RenderComponent* renderComponent;
	int spellType;
	Entity* target;
public:
	Spell(sf::Uint32 id);
	~Spell();

	void handleEvent(GameEvent* event) override;
	EntityType getType() override;
	EntityCategory getEntityCategory() override;
	uint16 getCollisionMask() override;

	void update(sf::Time elapsedTime, Map* map, Game* g) override;

	void loadFromJson(json jsonData);

	Spell* clone() const;

	PositionComponent* getPositionComponent() const {
		return positionComponent;
	}

	RenderComponent* getRenderComponent() const {
		return renderComponent;
	}

	void setTarget(Entity* target) {
		this->target = target;
	}

	Entity* getTarget() const {
		return target;
	}

	void setSpellType(int type) {
		this->spellType = type;
	}

	int getSpellType() const {
		return spellType;
	}
};


#endif
