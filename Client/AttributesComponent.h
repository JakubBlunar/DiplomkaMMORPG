#ifndef ATTRIBUTES_COMPONENT_H
#define ATTRIBUTES_COMPONENT_H

#include "Component.h"
#include <vector>

enum class EntityAttributeType : unsigned char;

class AttributesComponent :
	public Component
{
	std::vector<float> attributes;
	std::vector<float> bonuses;
public:
	AttributesComponent();
	~AttributesComponent();
	void update(sf::Time elapsedTime, Entity* entity, Map* map) override;
	ComponentType getType() override;

	float getAttribute(EntityAttributeType type) const;
	void setAttribute(EntityAttributeType type, float value);
	void setAttributeByIndex(int index, float value);

	float getBonus(EntityAttributeType type) const; 
	void setBonus(EntityAttributeType type, float value);
	void setBonusByIndex(int index, float value);

	float modifyAttribute(EntityAttributeType type, float value);
	int attributeTypeToInt(EntityAttributeType type) const;

	int getXpForLevel(int level) const ;
};

#endif