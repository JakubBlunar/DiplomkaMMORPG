#include "Entity.h"



Entity::Entity()
{
}


Entity::~Entity()
{
}

Component* Entity::getComponent(ComponentType type) const
{
	for (Component* const component : components)
	{
		if(component->getType() == type)
		{
			return component;
		}
	}
	return nullptr;
}
