#ifndef ENTITY_ATTRIBUTES_H
#define ENTITY_ATTRIBUTES_H
#include <vector>
#include "../Shared/EntityConstants.h"

namespace s {
	class EntityAttributes
	{
	protected:
		EntityAttributes();
	public:
		std::vector<float> attributes;
		std::vector<float> attributeBonuses;

		float getAttribute(EntityAttributeType type, bool withBonus) const;
		float getAttributeByIndex(int index) const;
		float getAttributeBonusByIndex(int index) const;
		float getAttributeBonus(EntityAttributeType type) const;

		void setAttributeByIndex(int index, float value);
		void setAttribute(EntityAttributeType type, float value);
		void setAttributeBonusByIndex(int index, float value);
		void setAttributeBonus(EntityAttributeType type, float value);
		

		float modifyAttribute(EntityAttributeType type, float value);
		int attributeTypeToInt(EntityAttributeType type) const;

		void recalcMaxHealth();
		void recalcMaxMana();
		int recalcLevel();
		float getXpForLevel(int level) const;
	};
}

#endif