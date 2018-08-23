#include "RenderSprite.h"
#include "ResourceHolder.h"
#include <iostream>

void RenderSprite::load(std::string textureName, sf::Vector2i size, sf::Vector2i position) {
	try {
		ResourceHolder<sf::Texture>* resourceManager = ResourceHolder<sf::Texture>::instance();
		this->setTexture(resourceManager->get(textureName));
		this->setTextureRect(sf::IntRect(position, size));
	}
	catch (...) {
		std::cout << "cannot load " << textureName << std::endl;
	}
}
