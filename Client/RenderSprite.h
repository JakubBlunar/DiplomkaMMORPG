#ifndef RENDER_SPRITE_H
#define RENDER_SPRITE_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>

#include <string>

class RenderSprite : public sf::Sprite {
public:
	void load(std::string textureName, sf::Vector2i size, sf::Vector2i position);
};

#endif
