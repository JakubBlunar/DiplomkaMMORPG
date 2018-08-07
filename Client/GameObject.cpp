#include "GameObject.h"
#include "PositionComponent.h"
#include "Globals.h"
#include "JsonLoader.h"
#include "ResourceHolder.h"


GameObject::GameObject(long long id, std::string file) : Entity(id)
{
	positionComponent = new PositionComponent();
	components.push_back(positionComponent);
	positionComponent->setSpeed(0);
	positionComponent->setPosition(sf::Vector2f(200.f, 300.f));

	renderComponent = new RenderComponent();
	components.push_back(renderComponent);

	this->loadFromJson(file);
}


GameObject::~GameObject()
{
}

void GameObject::handleEvent(GameEvent* event)
{

}

void GameObject::update(sf::Time elapsedTime, Map* map)
{
	body->SetLinearVelocity(b2Vec2(0, 0));
}

void GameObject::loadFromJson(const std::string& file)
{

	json jsonFile = JsonLoader::instance()->loadJson(file);

	//name
	this->name = jsonFile.value("name", "Undefined name");

	json position = jsonFile["position"].get<json::object_t>();
	if (position.is_object())
	{
		BodyType bodyType = static_cast<BodyType>(position["bodyType"].get<json::number_integer_t>());
		positionComponent->setBodyType(bodyType);
		float width = position["width"].get<json::number_integer_t>();
		float height = position["height"].get<json::number_integer_t>();
		positionComponent->setSize(sf::Vector2f(width, height));


	}else
		throw "cannot load gameObject from file " + file;
	
	
	json render = jsonFile["render"].get<json::object_t>();
	if (render.is_object())
	{	
		int width = render["width"].get<json::number_integer_t>();
		int height = render["height"].get<json::number_integer_t>();
		int renderOffsetX = render["renderOffsetX"].get<json::number_integer_t>();
		int renderOffsetY = render["renderOffsetY"].get<json::number_integer_t>();
		int spriteOffsetX = render["spriteOffsetX"].get<json::number_integer_t>();
		int spriteOffsetY = render["spriteOffsetY"].get<json::number_integer_t>();

		std::string textureFile = render["texture"].get<json::string_t>();

		Animation idleAnimation;
	    idleAnimation.setSpriteSheet(ResourceHolder<sf::Texture>::instance()->get(textureFile));
	    idleAnimation.addFrame(sf::IntRect(spriteOffsetX, spriteOffsetY, width, height));

		renderComponent->setSize(sf::Vector2i(width, height));
		renderComponent->setOffset(sf::Vector2i(renderOffsetX, renderOffsetY));

		std::string animationName = "iddle";
		renderComponent->addAnimation(animationName, &idleAnimation);
		renderComponent->changeAnimation("iddle");
	}else
		throw "cannot load gameObject from file " + file;
}

EntityType GameObject::getType()
{
	return EntityType::GAMEOBJECT;
}

RenderComponent* GameObject::getRenderComponent() const
{
	return renderComponent;
}

PositionComponent* GameObject::getPositionComponent() const
{
	return positionComponent;
}
