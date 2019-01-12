#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

class Animation {
public:
	Animation();

	void addFrame(sf::IntRect rect);
	void setSpriteSheet(const sf::Texture& texture);
	const sf::Texture* getSpriteSheet() const;
	std::size_t getSize() const;
	const sf::IntRect& getFrame(std::size_t n) const;
	void setFrameTime(sf::Time time);
	sf::Time getFrameTime() const;

	Animation* clone() const;
private:
	std::vector<sf::IntRect> frames;
	const sf::Texture* texture;
	sf::Time frameTime;
};

#endif
