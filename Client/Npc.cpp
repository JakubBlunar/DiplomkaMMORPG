#include "Npc.h"
#include "Globals.h"
#include "JsonLoader.h"
#include "ResourceHolder.h"
#include "EventDispatcher.h"
#include "EventNpcMovementChange.h"
#include "EventNpcsMovementChange.h"
#include "PositionComponent.h"
#include "RenderComponent.h"
#include "AttributesComponent.h"
#include "EventNpcStatusChanged.h"
#include "EventAttributesChanged.h"

#include "Box2D/Box2D.h"

Npc::Npc(): Entity(-1), npcState(NpcState::IDLE), lastServerPosition() {
	type = -1;

	positionComponent = new PositionComponent();
	components.push_back(positionComponent);
	positionComponent->setSize(sf::Vector2f(32.f, 32.f));

	renderComponent = new RenderComponent();
	components.push_back(renderComponent);

	attributesComponent = new AttributesComponent();
	components.push_back(attributesComponent);
}

Npc::~Npc()
{
	unsubscribe();
	components.clear();

	delete positionComponent;
	delete renderComponent;
	delete attributesComponent;
}

void Npc::handleEvent(GameEvent* event) {
	switch (event->getId()) {
		case NPC_MOVEMENT_CHANGE: {
			EventNpcMovementChange* temp = (EventNpcMovementChange*) event;

			if(temp->spawnId != (int)id)
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
			
			/*
			positionComponent->setPosition(sf::Vector2f(temp->x, temp->y));
			positionComponent->setSpeed(temp->speed);
			positionComponent->setMovement(sf::Vector2f(temp->velX, temp->velY));
			if (body) {
				body->SetTransform(b2Vec2(temp->x * PIXTOMET, temp->y * PIXTOMET),body->GetAngle());
				body->SetLinearVelocity(b2Vec2(temp->velX * PIXTOMET, temp->velY * PIXTOMET));
			}
			*/

			//updateMovementAnimation();
			break;
		}
		case NPCS_MOVEMENT_CHANGE: {
			EventNpcsMovementChange* temp = (EventNpcsMovementChange*) event;
			auto found = temp->npcsMovements.find(id);
			if (found == temp->npcsMovements.end()) {
				return;
			}

			lastServerPosition = found->second;

			sf::Vector2f position = positionComponent->getPosition();
			if (sqrt(pow(position.x - lastServerPosition.x, 2) + pow(position.y - lastServerPosition.y, 2)) > 64) {
				positionComponent->setPosition(sf::Vector2f(lastServerPosition.x, lastServerPosition.y));
				if (body) {
					body->SetTransform(b2Vec2(lastServerPosition.x * PIXTOMET, lastServerPosition.y * PIXTOMET), body->GetAngle());
				}
			}
			break;
		}
		case NPC_STATUS_CHANGED: {
			EventNpcStatusChanged* temp = (EventNpcStatusChanged*) event;
			if(temp->spawnId != (int)id)
				return;

			npcState = temp->npcState;
			break;
		}
		case ATTRIBUTES_CHANGED: {
			EventAttributesChanged* temp = (EventAttributesChanged*) event;
			if (temp->entityType != NPC) 
				return;

			if (temp->spawnId != (int)id)
				return;

			for (auto change: temp->changes) {
				attributesComponent->setAttribute(change.first, change.second);
			}
			break;
		}
		default:
		break;
	}
}

EntityType Npc::getType()
{
	return EntityType::NPC;
}

EntityCategory Npc::getEntityCategory()
{
	return EntityCategory::NPC;
}

uint16 Npc::getCollisionMask() {
	return BOUNDARY | GAME_OBJECT;
}

void Npc::update(sf::Time elapsedTime, Map* map, Game* g) {
	Entity::update(elapsedTime, map, g);

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

	if (abs(neededMovement.x) < 2) {
		neededMovement.x = 0;
	}

	if (abs(neededMovement.y) < 2) {
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

RenderComponent* Npc::getRenderComponent() const {
	return renderComponent;
}

PositionComponent * Npc::getPositionComponent() const
{
	return positionComponent;
}

AttributesComponent* Npc::getAttributesComponent() const {
	return attributesComponent;
}

void Npc::updateMovementAnimation()
{
	sf::Vector2f movement = positionComponent->getMovement();
	sf::Vector2f position = positionComponent->getPosition();

	if (movement.x == 0 && movement.y == 0) {
		renderComponent->getCurrentAnimation()->stop();
	}
	else if (movement.x == 0 && movement.y < 0) // top
	{
		renderComponent->changeAnimation("up");
		renderComponent->getCurrentAnimation()->setLooped(true);
		if(body)
			body->SetTransform(b2Vec2(position.x * PIXTOMET, position.y * PIXTOMET), 180 * DEGTORAD);
	}
	else if (movement.x == 0 && movement.y > 0) //down
	{
		renderComponent->changeAnimation("down");
		renderComponent->getCurrentAnimation()->setLooped(true);
		if(body)
			body->SetTransform(b2Vec2(position.x * PIXTOMET, position.y * PIXTOMET), 0);
		
	}
	else if (movement.x < 0 && movement.y == 0) //left
	{
		renderComponent->changeAnimation("left");
		renderComponent->getCurrentAnimation()->setLooped(true);
		if(body)
			body->SetTransform(b2Vec2(position.x * PIXTOMET, position.y * PIXTOMET), 90 * DEGTORAD);
	}
	else if (movement.x > 0 && movement.y == 0) {
		renderComponent->changeAnimation("right");
		renderComponent->getCurrentAnimation()->setLooped(true);
		if(body)
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

void Npc::loadFromJson(json serverData)
{
	type = (int)serverData["type"].get<json::number_integer_t>();
	id = (int)serverData["spawnId"].get<json::number_integer_t>();
	float positionX = (float)serverData["positionX"].get<json::number_float_t>();
	float positionY = (float)serverData["positionY"].get<json::number_float_t>();

	positionComponent->setPosition(sf::Vector2f(positionX, positionY));
	lastServerPosition.x = positionX;
	lastServerPosition.y = positionY;

	float movementX = (float)serverData["movementX"].get<json::number_float_t>();
	float movementY = (float)serverData["movementY"].get<json::number_float_t>();
	lastServerPosition.velocityX = movementX;
	lastServerPosition.velocityY = movementY;


	positionComponent->setMovement(sf::Vector2f(movementX, movementY));

	json jsonData = JsonLoader::instance()->loadJson("Npcs/" + std::to_string(type));

	name = jsonData["name"].get<json::string_t>();
	npcState = static_cast<NpcState>(serverData["state"].get<json::number_integer_t>());

	std::string renderFile = jsonData["render"].get<json::string_t>();

	json animationData = JsonLoader::instance()->loadJson("Graphics/Npcs/" + std::to_string(type));

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
			ResourceHolder<sf::Texture>::instance()->get("Graphics/Npcs/" + std::to_string(type) + ".png"));
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
	updateMovementAnimation();

	json attributes = serverData["attributes"].get<json::array_t>();
	int index = 0;
	for (json::iterator it = attributes.begin(); it != attributes.end(); ++it) {
		attributesComponent->setAttributeByIndex(index, *it);
		index++;
	}
	subscribe();
}

NpcState Npc::getState() const {
	return npcState;
}

bool Npc::isHostile(Entity* entity) const {
	return entity->getType() == EntityType::PLAYER ? true: false;
}

void Npc::subscribe() {
	EventDispatcher<EventNpcsMovementChange>::addSubscriber(this);
	EventDispatcher<EventNpcMovementChange>::addSubscriber(this);
	EventDispatcher<EventNpcStatusChanged>::addSubscriber(this);
	EventDispatcher<EventAttributesChanged>::addSubscriber(this);
}

void Npc::unsubscribe() {
	EventDispatcher<EventNpcMovementChange>::removeSubscriber(this);
	EventDispatcher<EventNpcsMovementChange>::removeSubscriber(this);
	EventDispatcher<EventAttributesChanged>::removeSubscriber(this);
	EventDispatcher<EventNpcStatusChanged>::removeSubscriber(this);
}
