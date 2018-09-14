#ifndef ENTITY_CONSTANTS_H
#define ENTITY_CONSTANTS_H



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
	PLAYER_SENSOR = 0x020
};

#endif