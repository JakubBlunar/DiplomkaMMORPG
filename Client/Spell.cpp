
#include "Spell.h"
#include "PositionComponent.h"
#include "RenderComponent.h"
#include "JsonLoader.h"
#include "ResourceHolder.h"
#include "Globals.h"
#include "Account.h"

Spell::Spell(sf::Uint32 id): Entity(id), target(nullptr) {

	positionComponent = new PositionComponent();
	components.push_back(positionComponent);
	positionComponent->setSize(sf::Vector2f(32.f, 32.f));

	renderComponent = new RenderComponent();
	components.push_back(renderComponent);
}

Spell::~Spell() {
	
}

void Spell::handleEvent(GameEvent* event) {
	
}

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
	sf::Vector2f position = positionComponent->getPosition();

	float speed = 100.f;

	if (target) {
		sf::Vector2f nextMovement;
		PositionComponent* targetPoComponent = (PositionComponent*)target->getComponent(ComponentType::POSITION);
		sf::Vector2f targetPosition = targetPoComponent->getPosition();
		sf::Vector2f targetMovement = targetPoComponent->getMovement();

		float diffX = targetPosition.x - position.x;
		float diffY = targetPosition.y - position.y;
		float aspectRatio = abs(diffX / diffY);

		if (abs(diffX) > 10) {	
			if (diffX > 0) 
				nextMovement.x = speed * aspectRatio;
			else
				nextMovement.x = -speed * aspectRatio;	
		} else {
			nextMovement.x = targetMovement.x;
		}

		if (abs(diffY) > 10) {
			if (diffY > 0) 
				nextMovement.y = speed;
			else
				nextMovement.y = -speed;
		} else {
			nextMovement.y = targetMovement.y;
		}
	
		positionComponent->setMovement(nextMovement);
		body->SetLinearVelocity(b2Vec2(nextMovement.x * PIXTOMET, nextMovement.y * PIXTOMET));
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

	std::string animationFile = jsonData["entityAnimation"].get<json::string_t>();
	json animationData = JsonLoader::instance()->loadJson("Graphics/Spells/" + animationFile);

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
