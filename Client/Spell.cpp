#include "Spell.h"
#include "PositionComponent.h"
#include "RenderComponent.h"
#include "JsonLoader.h"
#include "ResourceHolder.h"
#include "Globals.h"
#include "Account.h"
#include <iostream>
#include "Map.h"

Spell::Spell(sf::Uint32 id): Entity(id), spellType(0), target(nullptr) {

	positionComponent = new PositionComponent();
	components.push_back(positionComponent);

	renderComponent = new RenderComponent();
	components.push_back(renderComponent);
}

Spell::~Spell() { }

void Spell::handleEvent(GameEvent* event) { }

EntityType Spell::getType() {
	return EntityType::SPELL;
}

EntityCategory Spell::getEntityCategory() {
	return EntityCategory::SPELL;
}

uint16 Spell::getCollisionMask() {
	return 0;
}

void Spell::update(sf::Time elapsedTime, Map* map, Game* g) {
	Entity::update(elapsedTime, map, g);

	sf::Vector2f movement = positionComponent->getMovement();
	if (target) {
		b2Vec2 actualPosition = body->GetPosition();
		b2Vec2 targetPosition = target->getBody()->GetPosition();

		b2Vec2 velocity = targetPosition - actualPosition;

		double distance = b2DistanceSquared(actualPosition, targetPosition);

		if (distance * METTOPIX < 5) {
			map->removeSpell(this);
			return;
		}

		velocity.Normalize();
		velocity *= speed * PIXTOMET;
		body->SetLinearVelocity(velocity);
		positionComponent->setMovement(sf::Vector2f(velocity.x * METTOPIX, velocity.y * METTOPIX));
	}


	if (movement.x == 0 && movement.y == 0) {
		renderComponent->changeAnimation("down");
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
}


void Spell::loadFromJson(json jsonData) {

	spellType = (int)jsonData["id"].get<json::number_integer_t>();
	float speed = (float)jsonData["spellSpeed"].get<json::number_integer_t>();

	std::string animationFile = jsonData["entityAnimation"].get<json::string_t>();
	json animationData = JsonLoader::instance()->loadJson("Graphics/Spells/" + animationFile);

	int offsetX = (int)animationData["offsetX"].get<json::number_integer_t>();
	int offsetY = (int)animationData["offsetY"].get<json::number_integer_t>();
	renderComponent->setOffset(sf::Vector2i(offsetX, offsetY));

	int width = (int)animationData["width"].get<json::number_integer_t>();
	int height = (int)animationData["height"].get<json::number_integer_t>();
	renderComponent->setSize(sf::Vector2i(width, height));
	
	this->speed = speed;
	positionComponent->setSize(sf::Vector2f((float)width, (float)height));
	positionComponent->setBodyType(BodyType::CIRCLE);

	json animations = animationData["animations"].get<json::array_t>();

	for (json::iterator animit = animations.begin(); animit != animations.end(); ++animit) {
		json animation = *animit;
		std::string name = animation["name"].get<json::string_t>();
		json steps = animation["steps"].get<json::array_t>();
		int frameTime = (int)animation["frameTime"].get<json::number_integer_t>();

		Animation* a = new Animation();
		a->setSpriteSheet(
			ResourceHolder<sf::Texture>::instance()->get("Graphics/Spells/" + animationFile + ".png"));
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
	renderComponent->getCurrentAnimation()->setLooped(true);

	positionComponent->setBodyType(BodyType::CIRCLE);
}

Spell* Spell::clone() const {

	Spell* clone = new Spell(-1);
	json jsonData = JsonLoader::instance()->loadJson("Spells/" + std::to_string(spellType));
	clone->loadFromJson(jsonData);

	/*PositionComponent* pc = clone->getPositionComponent();
	pc->setMovement(sf::Vector2f(0, 0));
	pc->setSize(positionComponent->getSize());
	pc->setSpeed(positionComponent->getSpeed());
	pc->setBodyType(pc->getBodyType());

	RenderComponent* rc = clone->getRenderComponent();

	std::map<std::string, Animation*>* animations = renderComponent->getPossibleAnimations();
	for (std::map<std::string, Animation*>::iterator it = animations->begin(); it != animations->end(); ++it) {
		rc->addAnimation(it->first, it->second->clone());
	}
	rc->setSize(renderComponent->getSize());
	rc->setOffset(renderComponent->getOffset());
	rc->changeAnimation(renderComponent->getCurrentAnimationName());*/

	return clone;
}
