#include "MovableSpell.h"
#include "ServerGlobals.h"
#include "Box2D/Box2D.h"
#include "EntityPosition.h"
#include <spdlog/spdlog.h>
#include "Map.h"
#include "SpellEventApplyEffects.h"
#include "Effect.h"

s::MovableSpell::MovableSpell(): speed(0) {
	entityType = EntityType::MOVABLE_SPELL;
}


s::MovableSpell::~MovableSpell()
{
}

void s::MovableSpell::update(sf::Time elapsedTime, s::Server* s, Map* map) {
	b2Body* body = position.getBody();
	if (target && body) {
		this->position.setPosition(sf::Vector2f(body->GetPosition().x * METTOPIX, body->GetPosition().y * METTOPIX));
		this->position.setMovement(sf::Vector2f(body->GetLinearVelocity().x * METTOPIX, body->GetLinearVelocity().y * METTOPIX));

		b2Body* targetBody = target->getBody();
		if (targetBody) {
			b2Vec2 actualPosition = body->GetPosition();
			b2Vec2 targetPosition = targetBody->GetPosition();
			b2Vec2 velocity = targetPosition - actualPosition;

			double distance = b2DistanceSquared(actualPosition, targetPosition);

			if (distance * METTOPIX < 5) {
				map->removeSpell(this);
				SpellEventApplyEffects * e = new SpellEventApplyEffects(this, target);
				e->executeTime = s->getServerTime();
				s->spellManager.queueEvent(e);
				return;
			}

			velocity.Normalize();
			velocity *= speed * PIXTOMET;
			body->SetLinearVelocity(velocity);
		}
	}
}

b2Body* s::MovableSpell::getBody() const {
	return position.getBody();
}

s::Spell* s::MovableSpell::clone() const {
	MovableSpell* clone = new MovableSpell();

	EntityPosition* clonedPosition = &clone->position;
	clonedPosition->setMovement(sf::Vector2f(0,0));
	clonedPosition->setBody(nullptr);
	clonedPosition->setLastMovement(sf::Vector2f(0,0));
	clonedPosition->setLocation(nullptr);
	clonedPosition->setMap(nullptr);
	clonedPosition->setMapId(0);
	clonedPosition->setPosition(sf::Vector2f(0,0));
	clonedPosition->setSize(position.getSize());
	

	for (Effect* const effect : effects) {
		clone->addEffect(effect->clone());
	}
	clone->spellInfo = spellInfo;
	clone->setSpeed(speed);

	return clone;
}

void s::MovableSpell::loadFromJson(json data) {
	std::string animationFile = data["entityAnimation"].get<json::string_t>();
	json animationData = JsonLoader::instance()->loadJson("Graphics/Spells/" + animationFile);


	int width = (int)animationData["width"].get<json::number_integer_t>();
	int height = (int)animationData["height"].get<json::number_integer_t>();
	position.setSize(sf::Vector2i(width, height));

	speed = (float) data["spellSpeed"].get<float_t>();

	Spell::loadFromJson(data);
}

sf::Vector2f s::MovableSpell::getPosition() const {
	return position.getPosition();
}
