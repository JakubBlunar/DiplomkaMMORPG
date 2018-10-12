#include "Npc.h"
#include "Globals.h"
#include "JsonLoader.h"
#include "ResourceHolder.h"


Npc::Npc(): Entity(-1)
{
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

void Npc::loadFromJson(json serverData)
{
	type = (int)serverData["type"].get<json::number_integer_t>();
	id = (int)serverData["spawnId"].get<json::number_integer_t>();
	float positionX = (float)serverData["positionX"].get<json::number_float_t>();
	float positionY = (float)serverData["positionY"].get<json::number_float_t>();

	positionComponent->setPosition(sf::Vector2f(positionX, positionY));

	float movementX = (float)serverData["movementX"].get<json::number_float_t>();
	float movementY = (float)serverData["movementY"].get<json::number_float_t>();

	positionComponent->setMovement(sf::Vector2f(movementX, movementY));

	float speed = (float)serverData["speed"].get<json::number_float_t>();
	positionComponent->setSpeed(speed);

	json jsonData = JsonLoader::instance()->loadJson("Npcs/" + std::to_string(type));

	name = jsonData["name"].get<json::string_t>();
		
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
}
