#include "Npc.h"
#include "Globals.h"


Npc::Npc(sf::Uint32 spawnId, int type): Entity(spawnId)
{
	this->type = type;
	positionComponent = new PositionComponent();
	components.push_back(positionComponent);
	positionComponent->setSize(sf::Vector2f(32.f, 32.f));

	renderComponent = new RenderComponent();
	components.push_back(renderComponent);
}

Npc::~Npc()
{
}

void Npc::handleEvent(GameEvent* event) {
	
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
	
}

RenderComponent* Npc::getRenderComponent() const {
	return renderComponent;
}

PositionComponent * Npc::getPositionComponent() const
{
	return positionComponent;
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
