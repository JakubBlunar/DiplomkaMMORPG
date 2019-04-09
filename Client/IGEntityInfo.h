#ifndef IG_ENTITY_INFO_H
#define IG_ENTITY_INFO_H

#include "IGWindow.h"

class Entity;

class IGEntityInfo : public IGWindow
{
public:
	IGEntityInfo(std::string id, sf::Vector2f position);
	~IGEntityInfo();

	std::string id;
	Entity* entity;
	void setEntity(Entity* entity);
	Entity* getEntity() const;
protected:
	void render(Game* g, IGManager* manager) override;
};

#endif
