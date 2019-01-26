#include "NpcCommandCombat.h"
#include <Box2D/Dynamics/b2Body.h>
#include "Npc.h"
#include "ServerGlobals.h"
#include "EventAttributesChanged.h"
#include "EventNpcStatusChanged.h"
#include "NpcEventNpcIsIdle.h"

s::NpcCommandCombat::NpcCommandCombat(Npc* npc, Server* s) {
	this->npc = npc;
	this->server = s;

	target = npc->combat.target;
	maxDuration = sf::seconds(9999999999.f);
	finished = false;
}


s::NpcCommandCombat::~NpcCommandCombat() {}

void s::NpcCommandCombat::update(sf::Time elapsedTime, NpcUpdateEvents* npcUpdateEvents) {
	s::NpcCommand::update(elapsedTime, npcUpdateEvents);

	if (npc->spells.isCasting()) {
		return;
	}


	b2Body* body = npc->position.getBody();
	b2Body* targetBody = target->getBody();

	bool endCombat = false;
	b2Vec2 startCombatPosition;
	startCombatPosition.x = npc->combat.startCombatPosition.x * PIXTOMET;
	startCombatPosition.y = npc->combat.startCombatPosition.y * PIXTOMET;

	if (targetBody) {
		b2Vec2 actualPosition = body->GetPosition();
		b2Vec2 targetPosition = targetBody->GetPosition();
		b2Vec2 velocity = targetPosition - actualPosition;

		double distance = b2DistanceSquared(actualPosition, targetPosition);
		if (distance * METTOPIX < 50) {
			velocity = b2Vec2(0, 0);
		}

		if (distance > 500) {
			endCombat = true;
			velocity = b2Vec2(0, 0);
		}

		double distanceFromStartCombat = b2DistanceSquared(startCombatPosition, targetPosition);
		if (distanceFromStartCombat > 800) {
			endCombat = true;
			velocity = b2Vec2(0, 0);
		}

		velocity.Normalize();
		velocity *= npc->attributes.getAttribute(EntityAttributeType::SPEED, true) * PIXTOMET;
		body->SetLinearVelocity(velocity);
	}

	npc->position.setPosition(sf::Vector2f(body->GetPosition().x * METTOPIX, body->GetPosition().y * METTOPIX));
	npc->setMovement(sf::Vector2f(body->GetLinearVelocity().x * METTOPIX, body->GetLinearVelocity().y * METTOPIX),
	                 nullptr);

	if (endCombat) {
		finished = true;
		npc->combat.reset();
		EventAttributesChanged* resetAttributesEvent = new EventAttributesChanged();
		resetAttributesEvent->entityType = NPC;
		resetAttributesEvent->spawnId = npc->getSpawnId();

		float newHp = npc->getAttribute(EntityAttributeType::BASE_HP, true);
		float newMana = npc->getAttribute(EntityAttributeType::BASE_MP, true);
		resetAttributesEvent->setChange(EntityAttributeType::HP, newHp);
		resetAttributesEvent->setChange(EntityAttributeType::MP, newMana);
		npc->attributes.setAttribute(EntityAttributeType::HP, newHp);
		npc->attributes.setAttribute(EntityAttributeType::MP, newMana);

		EventNpcStatusChanged* statusChangedEvent = new EventNpcStatusChanged();
		statusChangedEvent->npcState = NpcState::COMBAT;
		statusChangedEvent->spawnId = npc->getSpawnId();

		npc->position.getMap()->sendEventToAllPlayers(statusChangedEvent);
		npc->position.getMap()->sendEventToAllPlayers(resetAttributesEvent);

		NpcCommand* c = npc->getNpcCommand();

		npc->setNpcCommand(nullptr);

		NpcEventNpcIsIdle * iddleEvent = new NpcEventNpcIsIdle();
		iddleEvent->npc = npc;
		dispatchFinishEvent(iddleEvent);

		delete c;
	}
}
