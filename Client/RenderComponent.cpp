#include "RenderComponent.h"
#include <cassert>

RenderComponent::RenderComponent()
{
	currentAnimation = new AnimatedSprite(sf::seconds(1000000000000.f), true, false);
}


RenderComponent::~RenderComponent()
{
}

void RenderComponent::update(sf::Time elapsedTime, Entity* entity, Map* map)
{
	currentAnimation->update(elapsedTime);
}

ComponentType RenderComponent::getType()
{
	return ComponentType::RENDER;
}

void RenderComponent::addAnimation(std::string animationName, Animation* animation)
{
	auto found = possibleAnimations.find(animationName);
	if(found != possibleAnimations.end())
	{
		throw "Animation already exists with name " + animationName;
	}

	auto inserted = possibleAnimations.insert(std::make_pair(animationName, animation));
	assert(inserted.second);
}

void RenderComponent::changeAnimation(std::string animationName)
{
	auto found = possibleAnimations.find(animationName);
	if (found == possibleAnimations.end())
	{
		throw "Animation dont exists with name " + animationName;
	}

	if(currentAnimation != nullptr)
		currentAnimation->stop();
	
	Animation* animation = found->second;
	currentAnimation->setAnimation(*animation);
	currentAnimation->play();
}

sf::Vector2i RenderComponent::getSize() const
{
	return size;
}

void RenderComponent::setSize(sf::Vector2i size)
{
	this->size = size;
}

sf::Vector2i RenderComponent::getOffset() const
{
	return offset;
}

void RenderComponent::setOffset(sf::Vector2i offset)
{
	this->offset = offset;
}

AnimatedSprite * RenderComponent::getCurrentAnimation() const
{
	return currentAnimation;
}
