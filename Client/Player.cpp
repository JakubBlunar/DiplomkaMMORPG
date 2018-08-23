#include "Player.h"
#include <SFML/Window/Keyboard.hpp>
#include "Globals.h"
#include "RenderComponent.h"
#include "../Shared/CharacterConstants.h"
#include "JsonLoader.h"
#include "ResourceHolder.h"
#include <iostream>


Player::Player(bool playerControlled) : Entity(0) {
	this->playerControlled = playerControlled;
	lastMovement = sf::Vector2f(0, 0);
	positionComponent = new PositionComponent();
	components.push_back(positionComponent);
	positionComponent->setPosition(sf::Vector2f(120, 120.f));
	positionComponent->setSpeed(70);
	positionComponent->setSize(sf::Vector2f(32.f, 32.f));

	renderComponent = new RenderComponent();
	components.push_back(renderComponent);
}

Player::~Player() {
	components.clear();
	delete positionComponent;
}

void Player::handleEvent(GameEvent* event) {

}

void Player::update(sf::Time elapsedTime, Map* map) {
	Entity::update(elapsedTime, map);

	if (this->playerControlled) {
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

		if (lastMovement != movement) {
			if (movement.x == 0 && movement.y == 0) {
				renderComponent->getCurrentAnimation()->stop();
			}
			else if (movement.x == 0 && movement.y < 0) // top
			{
				renderComponent->changeAnimation("up");
				renderComponent->getCurrentAnimation()->setLooped(true);
			}
			else if (movement.x == 0 && movement.y > 0) //down
			{
				renderComponent->changeAnimation("down");
				renderComponent->getCurrentAnimation()->setLooped(true);
			}
			else if (movement.x < 0 && movement.y == 0) //left
			{
				renderComponent->changeAnimation("left");
				renderComponent->getCurrentAnimation()->setLooped(true);
			}
			else if (movement.x > 0 && movement.y == 0) {
				renderComponent->changeAnimation("right");
				renderComponent->getCurrentAnimation()->setLooped(true);
			}
			else if (movement.x > 0) {
				renderComponent->changeAnimation("right");
				renderComponent->getCurrentAnimation()->setLooped(true);
			}
			else if (movement.x < 0) {
				renderComponent->changeAnimation("left");
				renderComponent->getCurrentAnimation()->setLooped(true);
			}

			lastMovement = movement;
			body->SetLinearVelocity(b2Vec2(movement.x * PIXTOMET, movement.y * PIXTOMET));
		}
	}
}

EntityType Player::getType() {
	return EntityType::PLAYER;
}

EntityCategory Player::getEntityCategory() {
	if (playerControlled)
		return EntityCategory::PLAYER;
	return EntityCategory::ENEMY_PLAYER;
}

void Player::loadFromJson(json jsonData) {
	id = jsonData["id"].get<json::number_integer_t>();
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
}
