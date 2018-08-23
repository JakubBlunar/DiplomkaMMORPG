#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include "RenderSprite.h"

class Field {
	std::vector<RenderSprite*> layers;
public:
	Field();
	~Field();

	void init();
	std::vector<RenderSprite*>* getLayers();
};


#endif
