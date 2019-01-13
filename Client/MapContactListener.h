#ifndef MAP_CONTACT_LISTENER_H
#define MAP_CONTACT_LISTENER_H

#include <Box2D/Dynamics/b2WorldCallbacks.h>

class Map;

class MapContactListener : public b2ContactListener {
	void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
public:
	Map* map;

	void setMap(Map* map) {
		 this->map = map;
	}
 };

#endif
