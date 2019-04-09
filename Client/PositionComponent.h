#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include "Component.h"
#include <SFML/System/Vector2.hpp>
#include "../Shared/EntityConstants.h"

class PositionComponent :
	public Component {
public:
	PositionComponent();
	~PositionComponent();

	void update(sf::Time elapsedTime, Entity* entity, Map* map) override;
	ComponentType getType() override;
	void setMovementDirection(sf::Vector2f direction, float speed);

	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition() const;

	sf::Vector2f getMovement() const;
	void setMovement(sf::Vector2f movement);

	sf::Vector2f getSize() const;
	void setSize(sf::Vector2f size);

	void setBodyType(BodyType type);
	BodyType getBodyType() const;

	bool isMovingUp;
	bool isMovingDown;
	bool isMovingRight;
	bool isMovingLeft;
private:
	sf::Vector2f movement;
	sf::Vector2f lastMovement;
	sf::Vector2f position;
	sf::Vector2f size;
	BodyType bodyType = BodyType::RECTANGLE;
};

#endif
