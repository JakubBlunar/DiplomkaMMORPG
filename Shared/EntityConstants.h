#ifndef ENTITY_CONSTANTS_H
#define ENTITY_CONSTANTS_H
#include <SFML/Graphics/Font.hpp>

enum class BodyType {
	RECTANGLE = 0,
	CIRCLE = 1,
};

enum EntityCategory {
	BOUNDARY = 0x0001,
	PLAYER = 0x0002,
	ENEMY_PLAYER = 0x0004,
	GAME_OBJECT = 0x0008,
	SENSOR = 0x0010,
	PLAYER_SENSOR = 0x0200,
	NPC = 0x0400
};

struct MovementData {
	float x;
	float y;
	float velocityX;
	float velocityY;
};

enum class EntityAttributeType: sf::Uint8 {
	HP = 0,
	MP = 1,
	STRENGTH = 2,
	AGILITY = 3,
	INTELECT = 4,
	SPIRIT = 5,
	STAMINA = 6,
	ARMOR = 7,
	EXPERIENCE = 9,
	MONEY = 10,
	BASE_HP = 11,
	BASE_MP = 12,
	COUNT = 13
};

#endif