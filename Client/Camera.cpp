#include "Camera.h"
#include <SFML/Window/Mouse.hpp>
#include "Game.h"
#include "SceneManager.h"
#include <iostream>
#include "Globals.h"

#define MIN_SCALE 0.8f
#define MAX_SCALE 1.3f

Camera::Camera():
	resolution(1360, 768),
	offset(16, 16),
	scale(1),
	rotation(0),
	lastView(0, 0, 0, 0) {
	view.setSize(resolution);
	view.setCenter(resolution.x / 2, resolution.y / 2);

	float temp = 6;
	playerBorders = sf::FloatRect(resolution.x / temp, resolution.y / temp, resolution.x - 2 * (resolution.x / temp),
	                              resolution.y - 2 * (resolution.y / temp));
}


Camera::~Camera() {
}

void Camera::update(sf::Time elapsedTime, Game* g) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		scale = 1;
		adjustScale(0, g);
	}

	if (g->sceneManager->getTypeOfActualScene() == SceneType::GAMEPLAY) {
		if (offset.y < 16) {
			offset.y = 16;
		}
		if (offset.x < 16) {
			offset.x = 16;
		}

		PositionComponent* p = (PositionComponent*)g->getAccount()->getPlayerEntity()->getComponent(ComponentType::POSITION);
		sf::Vector2f position = p->getPosition();

		if (playerBorders.left + playerBorders.width < position.x - offset.x) {
			offset.x = position.x - (playerBorders.left + playerBorders.width);

			if (offset.x + resolution.x >= g->getMap()->getWidth() * FIELD_SIZE + 16) {
				offset.x = g->getMap()->getWidth() * FIELD_SIZE - resolution.x + 16;
			}
		}

		if (playerBorders.left > position.x - offset.x) {
			offset.x = position.x - playerBorders.left;
			if (offset.x < 16) {
				offset.x = 16;
			}
		}

		if (playerBorders.top + playerBorders.height < position.y - offset.y) {
			offset.y = position.y - (playerBorders.top + playerBorders.height);
			if (offset.y + resolution.y > g->getMap()->getHeight() * FIELD_SIZE + 16) {
				offset.y = g->getMap()->getHeight() * FIELD_SIZE - resolution.y + 16;
			}
		}

		if (playerBorders.top > position.y - offset.y) {
			offset.y = position.y - playerBorders.top;
			if (offset.y < 16) {
				offset.y = 16;
			}
		}

		sf::FloatRect viewPort = sf::FloatRect(offset, resolution);

		if (viewPort != lastView) {
			std::cout << "Viewport changed 2 " << viewPort.left << " " << viewPort.top << std::endl;
			lastView = viewPort;
			view.reset(viewPort);
			view.zoom(scale);
			g->window.setView(view);
		}
	}
	else {
		offset = sf::Vector2f(0, 0);
		sf::FloatRect viewPort = sf::FloatRect(offset, resolution);
		if (viewPort != lastView) {
			std::cout << "Viewport changed" << std::endl;
			lastView = viewPort;
			view.reset(viewPort);
			g->window.setView(view);
		}

	}


}

void Camera::adjustScale(float delta, Game* g) {
	if (scale + delta < MIN_SCALE) {
		scale = MIN_SCALE;
	}
	else if (scale + delta > MAX_SCALE) {
		scale = MAX_SCALE;
	}
	else {
		scale += delta;
	}
	view.reset(sf::FloatRect(offset, resolution));
	view.zoom(scale);
	g->window.setView(view);
}

void Camera::adjustRotation(float delta, Game* g) {
	if (rotation + delta < 0) {
		rotation = 360 - rotation + delta;
		return;
	}
	if (rotation + delta > 360) {
		rotation = rotation - 360 + delta;
		return;
	}
	rotation += delta;
	view.setRotation(rotation);
	g->window.setView(view);
}

sf::Vector2f Camera::getResolution() const {
	return resolution;
}

sf::Vector2f Camera::getOffset() const {
	return offset;
}

sf::View* Camera::getView() {
	return &view;
}
