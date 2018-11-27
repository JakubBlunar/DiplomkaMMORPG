#include "EntityAttributes.h"



s::EntityAttributes::EntityAttributes()
{
	int count = attributeTypeToInt(EntityAttributeType::COUNT);
	for (int i = 0; i < count; i++) {
		attributes.push_back(0.0);
	}
}

float s::EntityAttributes::getAttribute(EntityAttributeType type) const {
	return attributes[attributeTypeToInt(type)];
}

float s::EntityAttributes::getAttributeByIndex(int index) const {
	return attributes[index];
}

void s::EntityAttributes::setAttributeByIndex(int index, float value)
{
	attributes[index] = value;
}

void s::EntityAttributes::setAttribute(EntityAttributeType type, float value) {
	attributes[attributeTypeToInt(type)] = value;
}

float s::EntityAttributes::modifyAttribute(EntityAttributeType type, float value) {
	int index = attributeTypeToInt(type);
	attributes[index] += value;
	return attributes[index];
}

int s::EntityAttributes::attributeTypeToInt(EntityAttributeType type) const
{
	return static_cast<int>(type);
}


int s::EntityAttributes::recalcLevel()
{
	float experience = getAttribute(EntityAttributeType::EXPERIENCE); 
	float temp;
	int level = 0;

	do {
		level++;
		temp = getXpForLevel(level);
	}
	while(temp <= experience);

	setAttribute(EntityAttributeType::LEVEL, level);
	return level;
}

float s::EntityAttributes::getXpForLevel(int level) const {
	return 25 * level * level - 25 * level;
}
