#include "Camera.h"
#include <SFML/Window/Mouse.hpp>

#define MIN_SCALE 0.8f
#define MAX_SCALE 1.3f
Camera::Camera():
	resolution(1360, 768),
	offset(0, 0),
	scale(1),
	rotation(0)
{
	view.setSize(resolution);
	view.setCenter(resolution.x / 2 , resolution.y /2 );
}


Camera::~Camera()
{
}

void Camera::update(sf::Time elapsedTime, Game* g)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
	   scale = 1;
	}

	view.reset(sf::FloatRect(offset, resolution));
	view.zoom(scale);
	view.setRotation(rotation);
}

void Camera::adjustScale(float delta)
{
	if(scale + delta < MIN_SCALE)
	{
		scale = MIN_SCALE;
		return;
	}

	if (scale + delta > MAX_SCALE)
	{
		scale = MAX_SCALE;
		return;
	}

	scale += delta;
}

void Camera::adjustRotation(float delta)
{
	if(rotation + delta < 0)
	{
		rotation = 360 - rotation + delta;
		return;
	}
	if (rotation + delta > 360)
	{
		rotation = rotation - 360 + delta;
		return;
	}
	rotation += delta;
}

sf::View* Camera::getView()
{
	return &view;
}
