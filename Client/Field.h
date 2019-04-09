#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "RenderSprite.h"

class Field {
	std::vector<RenderSprite*> layers;
public:
	Field();
	~Field();

	void init();
	void addLayer(RenderSprite* sprite);
	std::vector<RenderSprite*>* getLayers();
};


#endif
