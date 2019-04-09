#ifndef CHARACTER_CONSTANTS_H
#define CHARACTER_CONSTANTS_H

enum class CharacterType {
	MALE_KNIGHT = 1,
	FEMALE_KNIGHT = 2,
};

enum class CharacterFaction {
	LIGHT = 1,
	DARK = 2
};

inline CharacterFaction operator|(CharacterFaction a, CharacterFaction b) {
	return static_cast<CharacterFaction>(static_cast<int>(a) | static_cast<int>(b));
}


#endif
