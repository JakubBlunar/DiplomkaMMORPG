#ifndef ENTITY_CONSTANTS_H
#define ENTITY_CONSTANTS_H

#include <SFML/System.hpp>

enum class BodyType {
	RECTANGLE = 0,
	CIRCLE = 1,
};

enum EntityCategory: sf::Uint32 {
	BOUNDARY = 0x0001,
	PLAYER = 0x0002,
	ENEMY_PLAYER = 0x0004,
	GAME_OBJECT = 0x0008,
	SENSOR = 0x0010,
	PLAYER_SENSOR = 0x0200,
	NPC = 0x0400,
	SPELL = 0x800
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
	BASE_HP = 2,
	BASE_MP = 3,
	LEVEL = 4,
	STRENGTH = 5,
	AGILITY = 6,
	INTELECT = 7,
	SPIRIT = 8,
	STAMINA = 9,
	ARMOR = 10,
	EXPERIENCE = 11,
	MONEY = 12,
	COUNT = 13
};

#endif