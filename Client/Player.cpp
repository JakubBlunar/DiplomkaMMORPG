#include "Player.h"
#include <SFML/Window/Keyboard.hpp>
#include "Globals.h"
#include "RenderComponent.h"
#include "JsonLoader.h"
#include "ResourceHolder.h"
#include "EventMovementChange.h"
#include "EventDispatcher.h"
#include "Map.h"
#include "Game.h"
#include <iostream>
#include "EntityPrototypes.h"
#include "Scene.h"
#include "EventPlayerStartCastSpell.h"
#include "Utils.h"
#include "EventAttributesChanged.h"
#include "EventAutoattackPlayer.h"


Player::Player(bool playerControlled) : Entity(0) {
	this->playerControlled = playerControlled;
	lastMovement = sf::Vector2f(0, 0);

	positionComponent = new PositionComponent();
	components.push_back(positionComponent);
	positionComponent->setSize(sf::Vector2f(32.f, 32.f));

	renderComponent = new RenderComponent();
	components.push_back(renderComponent);

	attributesComponent = new AttributesComponent();
	components.push_back(attributesComponent);

	sendingTime = sf::Time::Zero;

	EventDispatcher<EventMovementChange>::addSubscriber(this);
	EventDispatcher<EventAttributesChanged>::addSubscriber(this);
}

Player::~Player() {
	EventDispatcher<EventMovementChange>::removeSubscriber(this);
	EventDispatcher<EventAttributesChanged>::removeSubscriber(this);
	components.clear();

	delete positionComponent;
	delete attributesComponent;
	delete renderComponent;
}

void Player::handleEvent(GameEvent* event) {
	switch (event->getId()) {
		case MOVEMENT: {
			EventMovementChange* temp = (EventMovementChange*)event;

			if (temp->playerId != id || playerControlled)
				return;

			lastServerPosition.velocityX = temp->velX;
			lastServerPosition.velocityY = temp->velY;
			lastServerPosition.x = temp->x;
			lastServerPosition.y = temp->y;

			sf::Vector2f position = positionComponent->getPosition();
			if (sqrt(pow(position.x - lastServerPosition.x, 2) + pow(position.y - lastServerPosition.y, 2)) > 64) {
				positionComponent->setPosition(sf::Vector2f(temp->x, temp->y));
				if (body) {
					body->SetTransform(b2Vec2(temp->x * PIXTOMET, temp->y * PIXTOMET), body->GetAngle());
				}
			}
			/*positionComponent->setPosition(sf::Vector2f(temp->x, temp->y));
			positionComponent->setSpeed(temp->speed);
			positionComponent->setMovement(sf::Vector2f(temp->velX, temp->velY));
			if (body) {
				body->SetTransform(b2Vec2(temp->x * PIXTOMET, temp->y * PIXTOMET),body->GetAngle());
				body->SetLinearVelocity(b2Vec2(temp->velX * PIXTOMET, temp->velY * PIXTOMET));
			}
			
			updateMovementAnimation();*/
			break;
		}
		case ATTRIBUTES_CHANGED: {
			EventAttributesChanged* temp = (EventAttributesChanged*)event;
			if (temp->entityType != PLAYER)
				return;

			if (temp->spawnId != (int)id)
				return;

			for (auto change : temp->changes) {
				attributesComponent->setAttribute(change.first, change.second);
			}
			break;
		}
		default:
			break;
	}

}

void Player::update(sf::Time elapsedTime, Map* map, Game* g) {
	Entity::update(elapsedTime, map, g);

	body->SetLinearVelocity(b2Vec2(positionComponent->getMovement().x * PIXTOMET, positionComponent->getMovement().y * PIXTOMET));

	//cout << "VIEW: " << melleViewEntities.size() << " RANGE:" << melleRangeEntities.size() << endl;

	if (playerControlled) {
		sendingTime += elapsedTime;
		if (sendingTime > sf::seconds(2)) {
			sendingTime = sf::Time::Zero;
			sendPosition(g);

			//g->print("Sending player position  "+ std::to_string(elapsedTime.asMicroseconds()));
		}

		if (g->window && g->window->hasFocus()) {
			sf::Vector2f direction = sf::Vector2f(0.f, 0.f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				direction.y -= 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				direction.y += 1;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				direction.x -= 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				direction.x += 1;
			}
			setMovementDirection(direction, g);
		}
	}
	else {
		bool wasMovingLeft = positionComponent->isMovingLeft;
		bool wasMovingDown = positionComponent->isMovingDown;
		bool wasMovingUp = positionComponent->isMovingUp;
		bool wasMovingRight = positionComponent->isMovingRight;

		lastServerPosition.x += lastServerPosition.velocityX * elapsedTime.asSeconds();
		lastServerPosition.y += lastServerPosition.velocityY * elapsedTime.asSeconds();

		sf::Vector2f expectation;
		expectation.x = lastServerPosition.x + lastServerPosition.velocityX;
		expectation.y = lastServerPosition.y + lastServerPosition.velocityY;

		sf::Vector2f position = positionComponent->getPosition();

		sf::Vector2f neededMovement;
		neededMovement.x = expectation.x - position.x;
		neededMovement.y = expectation.y - position.y;

		if (abs(neededMovement.x) < 1) {
			neededMovement.x = 0;
		}

		if (abs(neededMovement.y) < 1) {
			neededMovement.y = 0;
		}

		positionComponent->setMovement(neededMovement);
		body->SetLinearVelocity(b2Vec2(neededMovement.x * PIXTOMET, neededMovement.y * PIXTOMET));

		if (wasMovingLeft != positionComponent->isMovingLeft ||
			wasMovingDown != positionComponent->isMovingDown ||
			wasMovingUp != positionComponent->isMovingUp ||
			wasMovingRight != positionComponent->isMovingRight) {
			updateMovementAnimation();
		}
	}
}

EntityType Player::getType() {
	return EntityType::PLAYER;
}

EntityCategory Player::getEntityCategory() {
	if (playerControlled)
		return PLAYER;
	return ENEMY_PLAYER;
}

uint16 Player::getCollisionMask() {
	return BOUNDARY | GAME_OBJECT;
}

void Player::updateMovementAnimation() {
	sf::Vector2f movement = positionComponent->getMovement();
	sf::Vector2f position = positionComponent->getPosition();

	if (movement.x == 0 && movement.y == 0) {
		renderComponent->getCurrentAnimation()->stop();
	}
	else if (movement.x == 0 && movement.y < 0) // top
	{
		renderComponent->changeAnimation("up");
		renderComponent->getCurrentAnimation()->setLooped(true);
		
		body->SetTransform(b2Vec2(position.x * PIXTOMET, position.y * PIXTOMET), 180 * DEGTORAD);
	}
	else if (movement.x == 0 && movement.y > 0) //down
	{
		renderComponent->changeAnimation("down");
		renderComponent->getCurrentAnimation()->setLooped(true);
		body->SetTransform(b2Vec2(position.x * PIXTOMET, position.y * PIXTOMET), 0);

	}
	else if (movement.x < 0 && movement.y == 0) //left
	{
		renderComponent->changeAnimation("left");
		renderComponent->getCurrentAnimation()->setLooped(true);
		body->SetTransform(b2Vec2(position.x * PIXTOMET, position.y * PIXTOMET), 90 * DEGTORAD);
	}
	else if (movement.x > 0 && movement.y == 0) {
		renderComponent->changeAnimation("right");
		renderComponent->getCurrentAnimation()->setLooped(true);
		body->SetTransform(b2Vec2(position.x * PIXTOMET, position.y * PIXTOMET), 270 * DEGTORAD);
	}
	else if (movement.x > 0) {
		renderComponent->changeAnimation("right");
		renderComponent->getCurrentAnimation()->setLooped(true);
	}
	else if (movement.x < 0) {
		renderComponent->changeAnimation("left");
		renderComponent->getCurrentAnimation()->setLooped(true);
	}

}

void Player::loadFromJson(json jsonData) {
	id = (int)jsonData["id"].get<json::number_integer_t>();
	name = jsonData["name"].get<json::string_t>();
	int type = (int)jsonData["type"].get<json::number_integer_t>();

	json animationData = JsonLoader::instance()->loadJson("Graphics/Characters/" + std::to_string(type));

	int offsetX = (int)animationData["offsetX"].get<json::number_integer_t>();
	int offsetY = (int)animationData["offsetY"].get<json::number_integer_t>();
	renderComponent->setOffset(sf::Vector2i(offsetX, offsetY));

	int width = (int)animationData["width"].get<json::number_integer_t>();
	int height = (int)animationData["height"].get<json::number_integer_t>();
	renderComponent->setSize(sf::Vector2i(width, height));

	json animations = animationData["animations"].get<json::array_t>();

	for (json::iterator animit = animations.begin(); animit != animations.end(); ++animit) {
		json animation = *animit;
		std::string name = animation["name"].get<json::string_t>();
		json steps = animation["steps"].get<json::array_t>();
		int frameTime = (int)animation["frameTime"].get<json::number_integer_t>();

		Animation* a = new Animation();
		a->setSpriteSheet(
			ResourceHolder<sf::Texture>::instance()->get("Graphics/Characters/" + std::to_string(type) + ".png"));
		a->setFrameTime(sf::milliseconds(frameTime));

		for (json::iterator stepIt = steps.begin(); stepIt != steps.end(); ++stepIt) {
			json step = *stepIt;

			int x = (int)step["x"].get<json::number_integer_t>();
			int y = (int)step["y"].get<json::number_integer_t>();
			a->addFrame(sf::IntRect(x, y, width, height));
		}

		renderComponent->addAnimation(name, a);
	}
	renderComponent->changeAnimation("down");

	float positionX = (float)jsonData["positionX"].get<json::number_float_t>();
	float positionY = (float)jsonData["positionY"].get<json::number_float_t>();

	positionComponent->setPosition(sf::Vector2f(positionX, positionY));
	lastServerPosition.x = positionX;
	lastServerPosition.y = positionY;

	float movementX = (float)jsonData["movementX"].get<json::number_float_t>();
	float movementY = (float)jsonData["movementY"].get<json::number_float_t>();

	positionComponent->setMovement(sf::Vector2f(movementX, movementY));
	lastServerPosition.velocityX = movementX;
	lastServerPosition.velocityY = movementY;

	json attributes = jsonData["attributes"].get<json::array_t>();
	std::string attributesS = jsonData.dump();
	int index = 0;
	for (json::iterator it = attributes.begin(); it != attributes.end(); ++it) {
		attributesComponent->setAttributeByIndex(index, *it);
		index++;
	}

	if (playerControlled) {
		json spellIds = jsonData["spells"].get<json::array_t>();
		EntityPrototypes* ep = EntityPrototypes::instance();
		for (json::iterator it = spellIds.begin(); it != spellIds.end(); ++it) {
			int spellId = *it;
			try {
				spells.push_back(ep->getSpellInfo(spellId));
			}
			catch (...) {}
		}
	}
}

b2Fixture* Player::getRange() const {
	return rangeView;
}

void Player::setRange(b2Fixture* fixture) {
	rangeView = fixture;
}

bool Player::isControlledByPlayer() const {
	return playerControlled;
}

void Player::addRangeEntity(Entity * entity, Game* g)
{
	rangeEntities.insert(entity);

	if (entity->getType() == EntityType::NPC){
		Npc* npc = (Npc*) entity;
		NpcState state = npc->getState();
		if (npc->autoAttackPlayer() && (npc->getState() == NpcState::IDLE || npc->getState() == NpcState::MOVING)) {
			EventAutoattackPlayer e;
			e.playerId = id;
			e.npcId = npc->getId();

			sf::Packet* packet = e.toPacket();
			g->packet_manager->sendPacket(packet);
			delete packet;
		}
	}
}

void Player::removeRangeEntity(Entity * entity)
{
	rangeEntities.erase(entity);
}

void Player::setMovementDirection(sf::Vector2f direction, Game* g) {
	positionComponent->setMovementDirection(direction, attributesComponent->getAttribute(EntityAttributeType::SPEED));
	sf::Vector2f movement = positionComponent->getMovement();

	if (lastMovement != movement) {
		updateMovementAnimation();
		lastMovement = movement;
		body->SetLinearVelocity(b2Vec2(movement.x * PIXTOMET, movement.y * PIXTOMET));
		sendPosition(g);
	}
}

AttributesComponent* Player::getAttributesComponent() const {
	return attributesComponent;
}

void Player::castSpell(SpellInfo* spellInfo, Map* map, Game* g) {
	auto exists = std::find(spells.begin(), spells.end(), spellInfo);
	if (exists == spells.end()) {
		return;
	}

	Entity* spellTarget = target;
	if (!target) {
		spellTarget = this;
	}

	switch (spellInfo->targetRestriction) {
		case SpellTargetRestriction::SELF: {
			if (spellTarget != this) {
				GameMessage* m = new GameMessage();
				m->message = "Target is invalid";
				m->displayTime = sf::seconds(1);
				g->addGameMessage(m);
				return;
			}
			break;
		}
		case SpellTargetRestriction::ENEMY: {
			if (!isHostile(spellTarget)) {
				GameMessage* m = new GameMessage();
				m->message = "Target is invalid";
				m->displayTime = sf::seconds(1);
				g->addGameMessage(m);
				return;
			}
			break;
		}
		case SpellTargetRestriction::FRIENDLY: {
			if (isHostile(spellTarget) ) {
				GameMessage* m = new GameMessage();
				m->message = "Target is invalid";
				m->displayTime = sf::seconds(1);
				g->addGameMessage(m);
				return;
			}
			break;
		}
	}

	float actualMana = attributesComponent->getAttribute(EntityAttributeType::MP);
	if (actualMana - spellInfo->manaCost < 0) {
		GameMessage* m = new GameMessage();
		m->message = "You do not have enough mana";
		m->displayTime = sf::seconds(1);
		g->addGameMessage(m);
		return;
	}

	if (spellInfo->castingTime > sf::Time::Zero) {
		sf::Vector2f movement = positionComponent->getMovement();
		if (movement != sf::Vector2f(0.0, 0.0)) {
			GameMessage* m = new GameMessage();
			m->message = "Can not cast while moving";
			m->displayTime = sf::seconds(1);
			g->addGameMessage(m);
			return;
		}
	}

	if (getGlobalCooldown(g) != nullptr || getCooldown(spellInfo->id, g) != nullptr) {
		GameMessage* m = new GameMessage();
		m->message = "Spell is not ready yet";
		m->displayTime = sf::seconds(1);
		g->addGameMessage(m);
		return;
	}

	AttributesComponent* targetAttributes = (AttributesComponent*)spellTarget->getComponent(ComponentType::ATTRIBUTES);
	if (targetAttributes && targetAttributes->getAttribute(EntityAttributeType::HP) <= 0) {
		GameMessage* m = new GameMessage();
		m->message = "Your target is dead.";
		m->displayTime = sf::seconds(1);
		g->addGameMessage(m);
		return;
	}

	bool canCast = spellTarget == this;
	std::string message;
	if (!canCast) {
		EntityToEntityRayCast* result = map->makeRayCast(this, spellTarget);
		if (result->closestEntity != target) {
			canCast = false;
			message = "Your target is not in your point of view";
		}
		else if (result->closestEntityDistance * METTOPIX > spellInfo->maxRange) {
			canCast = false;
			message = "Your target is too far";
		}
		else {
			canCast = true;
		}
		delete result;
	}

	if (canCast) {
		GameMessage* m = new GameMessage();
		m->message = "Casting spell" + spellInfo->name;
		m->displayTime = sf::seconds(2);
		g->addGameMessage(m);

		EventPlayerStartCastSpell e;
		e.spellId = spellInfo->id;
		e.startCastTimestamp = Utils::getActualUtcTime();

		EntityType type = spellTarget->getType();
		switch (type) {
			case EntityType::NPC: {
				e.target = SpellTarget::NPC;
				e.entityId = spellTarget->getId();
				break;
			}
			case EntityType::PLAYER: {
				e.target = SpellTarget::PLAYER;
				e.entityId = spellTarget->getId();
				break;
			}
			default: {
				GameMessage* m = new GameMessage();
				m->message = "Cannot target this entity";
				m->displayTime = sf::seconds(2);
				g->addGameMessage(m);
				return;
			}
		}

		if (spellInfo->castingTime > sf::Time::Zero) {
			setStartCastTime(g->getGameTime() + g->getLatency());
			setCastingSpell(spellInfo);
		}

		sf::Packet* p = e.toPacket();
		g->packet_manager->sendPacket(p);
		delete p;

		sf::Time gameTime = g->getGameTime();
		if (spellInfo->globalCooldownTime != sf::Time::Zero) {
			SpellCooldown* gCooldown = new SpellCooldown();
			gCooldown->cooldownFrom = gameTime;
			gCooldown->cooldownTo = gameTime + spellInfo->globalCooldownTime;
			setGlobalCooldown(gCooldown);
		}

		if (spellInfo->cooldownTime != sf::Time::Zero) {
			SpellCooldown* cooldown = new SpellCooldown();
			cooldown->cooldownFrom = gameTime;
			cooldown->cooldownTo = gameTime + spellInfo->cooldownTime;
			setCooldown(spellInfo->id, cooldown);
		}
	}
	else {
		GameMessage* m = new GameMessage();
		m->message = message;
		m->displayTime = sf::seconds(2);
		g->addGameMessage(m);
	}
}

SpellCooldown* Player::getGlobalCooldown(Game* g) const {
	if (globalCooldown && globalCooldown->cooldownTo > g->getGameTime()) {
		return globalCooldown;
	}
	return nullptr;
}

void Player::setGlobalCooldown(SpellCooldown* cooldown) {
	delete globalCooldown;
	globalCooldown = cooldown;
}

void Player::setCooldown(int spellType, SpellCooldown* cooldown) {
	auto found = spellCooldowns.find(spellType);
	if (found != spellCooldowns.end()) {
		SpellCooldown* old = found->second;
		delete old;
		spellCooldowns[spellType]  = cooldown;
	} else {
		spellCooldowns.insert(std::make_pair(spellType, cooldown));
	}
}

SpellCooldown* Player::getCooldown(int spellType, Game* g) const {
	auto found = spellCooldowns.find(spellType);
	if (found == spellCooldowns.end()) {
		return nullptr;
	}

	if (found->second->cooldownTo > g->getGameTime()) {
		return found->second;
	}
	return nullptr;
}

bool Player::isHostile(Entity* entity) const {

	if(entity == this) return false;

	if (entity->getType() == EntityType::PLAYER) {
		if (map->pvpEnabled()) {
			return true;
		}
		return false;
	}
	
	if (entity->getType() == EntityType::NPC) {
		return true;	
	}

	return false;
}


void Player::sendPosition(Game* g) const {
	sf::Vector2f position = positionComponent->getPosition();
	sf::Vector2f movement = positionComponent->getMovement();

	EventMovementChange e;
	e.playerId = id;
	e.x = position.x;
	e.y = position.y;
	e.velX = movement.x;
	e.velY = movement.y;

	sf::Packet* packet = e.toPacket();
	g->packet_manager->sendPacket(packet);

	delete packet;
}
