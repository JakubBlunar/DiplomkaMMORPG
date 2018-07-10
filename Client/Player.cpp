#include "Player.h"
#include <SFML/Window/Keyboard.hpp>


Player::Player(bool playerControlled) {
	this->playerControlled = playerControlled;

	positionComponent = new PositionComponent();
	components.push_back(positionComponent);
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
			direction.x -= 1;
		}
		positionComponent->setMovementDirection(direction);
	}
}

