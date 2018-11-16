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

		float getAttribute(EntityAttributeType type) const;
		float getAttributeByIndex(int index) const;
		void setAttribute(EntityAttributeType type, float value);
		float modifyAttribute(EntityAttributeType type, float value);
		int attributeTypeToInt(EntityAttributeType type) const;
	};
}

#endif