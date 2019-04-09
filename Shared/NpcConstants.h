#ifndef NPC_CONSTANTS_H
#define NPC_CONSTANTS_H

enum class NpcState {
	IDLE = 0,
	DEAD = 1,
	MOVING = 2,
	COMBAT = 3
};

enum NpcChangeType {
	STATE = 1,
	DMG_TAKEN = 2,
	HP = 2,
	MP = 3,

};

#endif
