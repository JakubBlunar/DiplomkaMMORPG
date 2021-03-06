﻿#include "GameObject.h"
#include "PositionComponent.h"
#include "Globals.h"
#include "JsonLoader.h"
#include "ResourceHolder.h"


GameObject::GameObject(sf::Uint32 id, std::string file) : Entity(id) {
	positionComponent = new PositionComponent();
	components.push_back(positionComponent);
	attributesComponent = new AttributesComponent();
	components.push_back(attributesComponent);

	attributesComponent->setAttribute(EntityAttributeType::SPEED, 0);

	renderComponent = new RenderComponent();
	components.push_back(renderComponent);

	this->loadFromJson("GameObjects/" + file);
}


GameObject::~GameObject() {}

void GameObject::handleEvent(GameEvent* event) {}

void GameObject::update(sf::Time elapsedTime, Map* map, Game* g) {
	Entity::update(elapsedTime, map, g);

}

void GameObject::loadFromJson(const std::string& file) {

	json jsonFile = JsonLoader::instance()->loadJson(file);

	//name
	this->name = jsonFile.value("name", "Undefined name");

	json position = jsonFile["position"].get<json::object_t>();
	if (position.is_object()) {
		BodyType bodyType = static_cast<BodyType>(position["bodyType"].get<json::number_integer_t>());
		positionComponent->setBodyType(bodyType);
		float width = (float)position["width"].get<json::number_float_t>();
		float height = (float)position["height"].get<json::number_float_t>();
		positionComponent->setSize(sf::Vector2f(width, height));
	}
	else
		throw "cannot load gameObject from file " + file;


	json render = jsonFile["render"].get<json::object_t>();
	if (render.is_object()) {
		int width = (int)render["width"].get<json::number_integer_t>();
		int height = (int)render["height"].get<json::number_integer_t>();
		int renderOffsetX = (int)render["renderOffsetX"].get<json::number_integer_t>();
		int renderOffsetY = (int)render["renderOffsetY"].get<json::number_integer_t>();



		std::string textureFile = render["texture"].get<json::string_t>();

		if (render.find("steps") == render.end()) {
			int spriteOffsetX = (int)render["spriteOffsetX"].get<json::number_integer_t>();
			int spriteOffsetY = (int)render["spriteOffsetY"].get<json::number_integer_t>();
			Animation* idleAnimation = new Animation();
			idleAnimation->setSpriteSheet(ResourceHolder<sf::Texture>::instance()->get(textureFile));
			idleAnimation->addFrame(sf::IntRect(spriteOffsetX, spriteOffsetY, width, height));
			renderComponent->addAnimation("iddle", idleAnimation);
			renderComponent->changeAnimation("iddle");
		}
		else {
			json steps = render["steps"].get<json::array_t>();
			int frameTime = (int)render["frameTime"].get<json::number_integer_t>();

			Animation* a = new Animation();
			a->setSpriteSheet(ResourceHolder<sf::Texture>::instance()->get(textureFile));
			a->setFrameTime(sf::milliseconds(frameTime));

			for (json::iterator stepIt = steps.begin(); stepIt != steps.end(); ++stepIt) {
				json step = *stepIt;

				int x = (int)step["x"].get<json::number_integer_t>();
				int y = (int)step["y"].get<json::number_integer_t>();
				a->addFrame(sf::IntRect(x, y, width, height));
			}

			renderComponent->addAnimation("iddle", a);
			renderComponent->changeAnimation("iddle");
			renderComponent->getCurrentAnimation()->setLooped(true);
			renderComponent->getCurrentAnimation()->play();
		}

		renderComponent->setSize(sf::Vector2i(width, height));
		renderComponent->setOffset(sf::Vector2i(renderOffsetX, renderOffsetY));


	}
	else
		throw "cannot load gameObject from file " + file;
}

EntityType GameObject::getType() {
	return EntityType::GAMEOBJECT;
}

RenderComponent* GameObject::getRenderComponent() const {
	return renderComponent;
}

PositionComponent* GameObject::getPositionComponent() const {
	return positionComponent;
}

EntityCategory GameObject::getEntityCategory() {
	return EntityCategory::GAME_OBJECT;
}

uint16 GameObject::getCollisionMask() {
	return PLAYER | ENEMY_PLAYER | PLAYER_SENSOR | NPC;
}
