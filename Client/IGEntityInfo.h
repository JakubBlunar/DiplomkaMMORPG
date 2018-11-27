#ifndef IG_ENTITY_INFO_H
#define IG_ENTITY_INFO_H

#include "IGWindow.h"

class Entity;
using namespace std;

class IGEntityInfo: public IGWindow
{
public:
	IGEntityInfo(string id, sf::Vector2f position);
	~IGEntityInfo();

	string id;
	Entity* entity;
	void setEntity(Entity* entity);
	Entity* getEntity() const;
protected:
	void render(Game* g, IGManager* manager) override;

	string convertFloatToString(float number, int precision) const;
};

#endif