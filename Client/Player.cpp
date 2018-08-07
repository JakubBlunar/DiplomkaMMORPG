#include "Player.h"
#include <SFML/Window/Keyboard.hpp>
#include "Globals.h"


Player::Player(long long id, bool playerControlled) : Entity(id)
{
	this->playerControlled = playerControlled;
	lastMovement = sf::Vector2f(0,0);
	positionComponent = new PositionComponent();
	components.push_back(positionComponent);
	positionComponent->setSpeed(32);
	positionComponent->setSize(sf::Vector2f(32.f, 32.f));
}

Player::~Player()
{
	components.clear();
	delete positionComponent;
}

void Player::handleEvent(GameEvent * event)
{

}

void Player::update(sf::Time elapsedTime, Map * map)
{
	Entity::update(elapsedTime, map);

	if (this->playerControlled)
	{
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
		positionComponent->setMovementDirection(direction);
		sf::Vector2f movement = positionComponent->getMovement();

		if(lastMovement != movement)
		{
			lastMovement = movement;
			body->SetLinearVelocity(b2Vec2(movement.x * PIXTOMET, movement.y * PIXTOMET));
		}		
	}
}

EntityType Player::getType()
{
	return EntityType::PLAYER;
}

