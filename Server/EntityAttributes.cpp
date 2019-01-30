#include "EntityAttributes.h"



s::EntityAttributes::EntityAttributes()
{
	int count = attributeTypeToInt(EntityAttributeType::COUNT);
	for (int i = 0; i < count; i++) {
		attributes.push_back(0.0f);
		attributeBonuses.push_back(0.0f);
	}
}

float s::EntityAttributes::getAttribute(EntityAttributeType type, bool withBonus = false) const {
	int index = attributeTypeToInt(type);
	if(withBonus) {
		return attributes[index] + attributeBonuses[index];
	}
	return attributes[index];
}

float s::EntityAttributes::getAttributeByIndex(int index) const {
	return attributes[index];
}

float s::EntityAttributes::getAttributeBonusByIndex(int index) const {
	return attributeBonuses[index];
}

float s::EntityAttributes::getAttributeBonus(EntityAttributeType type) const {
	return attributeBonuses[attributeTypeToInt(type)]; 
}

void s::EntityAttributes::setAttributeBonusByIndex(int index, float value) {
	attributeBonuses[index] = value;
}

void s::EntityAttributes::setAttributeBonus(EntityAttributeType type, float value) {
	attributeBonuses[attributeTypeToInt(type)] = value;
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

void s::EntityAttributes::recalcMaxHealth() {
	float level = getAttribute(EntityAttributeType::LEVEL);
	float stamina = getAttribute(EntityAttributeType::STAMINA, true);
	float maxHealth = level * 15 + stamina * 10;
	setAttribute(EntityAttributeType::BASE_HP, maxHealth);
}

void s::EntityAttributes::recalcMaxMana()
{
	float level = getAttribute(EntityAttributeType::LEVEL);
	float intelect = getAttribute(EntityAttributeType::INTELECT, true);
	float maxHealth = level * 10 + intelect * 5;
	setAttribute(EntityAttributeType::BASE_MP, maxHealth);
}


int s::EntityAttributes::recalcLevel()
{
	float experience = getAttribute(EntityAttributeType::EXPERIENCE); 
	float temp;
	int level = 0;

	do {
		level++;
		temp = getXpForLevel(level + 1);
	}
	while(temp < experience);

	setAttribute(EntityAttributeType::LEVEL, (float)level);
	return level;
}

float s::EntityAttributes::recalcArmor()
{
	float level = getAttribute(EntityAttributeType::LEVEL);
	float strength = getAttribute(EntityAttributeType::STRENGTH, true);

	float x = strength / (level * 0.8f);
	float cap = level / 2;
	float armor = (cap * x) / (x + 50);
	setAttribute(EntityAttributeType::ARMOR, armor);
	recalcResistance();
	return armor;
}

float s::EntityAttributes::recalcDodgeChance() {
	float level = getAttribute(EntityAttributeType::LEVEL);
	float agility = getAttribute(EntityAttributeType::AGILITY, true);

	float x = agility / (level * 0.8f);
	float armor = (0.75f * x) / (x + 8);
	setAttribute(EntityAttributeType::ARMOR, armor);
	recalcResistance();
	return armor;
}

float s::EntityAttributes::recalcResistance() {
	float level = getAttribute(EntityAttributeType::LEVEL);
	float armor = getAttribute(EntityAttributeType::ARMOR, true);

	float x = armor / (level * 0.7f);
	float resistance = (0.8f * x) / (x + 6);
	setAttribute(EntityAttributeType::RESISTANCE, resistance);
	return resistance;
}

float s::EntityAttributes::getBonusDamage(EntityAttributes attribute) const {
	
}

float s::EntityAttributes::getBonusHealing(EntityAttributes attribute) const {
	
}

float s::EntityAttributes::getXpForLevel(int level) const {
	return 15.f * level * level - 15.f * level;
}
