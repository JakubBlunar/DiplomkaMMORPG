#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>


class Game;

class Camera
{
public:
	Camera();
	~Camera();

	void update(sf::Time elapsedTime, Game* g);
	void adjustScale(float delta);
	void adjustRotation(float delta);
	sf::View* getView(); 
private:
	sf::Vector2f resolution;
	sf::Vector2f offset;
	float scale;
	float rotation;

	sf::View view;
};

#endif