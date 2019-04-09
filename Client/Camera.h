#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>


class Game;

class Camera {
public:
	Camera();
	~Camera();

	void update(sf::Time elapsedTime, Game* g);
	void adjustScale(float delta, Game* g);
	void adjustRotation(float delta, Game* g);
	sf::Vector2f getResolution() const;
	sf::Vector2f getOffset() const;
	sf::View* getView();
private:
	sf::Vector2f resolution;
	sf::Vector2f offset;
	float scale;
	float rotation;
	sf::FloatRect playerBorders;
	sf::FloatRect lastView;

	sf::View view;
};

#endif
