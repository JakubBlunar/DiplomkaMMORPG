#include "AttributesComponent.h"
#include "EntityConstants.h"


AttributesComponent::AttributesComponent()
{
	int count = attributeTypeToInt(EntityAttributeType::COUNT);
	for (int i = 0; i < count; i++) {
		attributes.push_back(0.0f);
	}
}

AttributesComponent::~AttributesComponent()
{
}

void AttributesComponent::update(sf::Time elapsedTime, Entity* entity, Map* map) {
	
}

ComponentType AttributesComponent::getType() {
	return ComponentType::ATTRIBUTES;
}

float AttributesComponent::getAttribute(EntityAttributeType type) const {
	return attributes[attributeTypeToInt(type)];
}

void AttributesComponent::setAttribute(EntityAttributeType type, float value) {
	attributes[attributeTypeToInt(type)] = value;
}

void AttributesComponent::setAttributeByIndex(int index, float value) {
	attributes[index] = value;
}

float AttributesComponent::modifyAttribute(EntityAttributeType type, float value) {
	int index = attributeTypeToInt(type);
	attributes[index] += value;
	return attributes[index];
}

int AttributesComponent::attributeTypeToInt(EntityAttributeType type) const {
	return static_cast<int>(type);
}
