#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"
#include <string>
#include <map>

#include "AnimatedSprite.h"

class RenderComponent :
	public Component
{
public:
	RenderComponent();
	~RenderComponent();

	void update(sf::Time elapsedTime, Entity* entity, Map* map) override;
	ComponentType getType() override;

	void addAnimation(std::string animationName, Animation* animation);
	void changeAnimation(std::string animationName);
	sf::Vector2i getSize() const;
	void setSize(sf::Vector2i size);
	sf::Vector2i getOffset() const;
	void setOffset(sf::Vector2i offset);

	AnimatedSprite* getCurrentAnimation() const;
private:
	std::map<std::string, Animation *> possibleAnimations;
	AnimatedSprite* currentAnimation;
	sf::Vector2i size;
	sf::Vector2i offset;
};

#endif