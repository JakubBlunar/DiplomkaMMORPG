#ifndef SPELL_CONSTANTS_H
#define SPELL_CONSTANTS_H


enum class SpellTarget : sf::Uint8 {
	PLAYER = 0,
	NPC = 1,
	POSITION = 2
};


enum class SpellType {
	MELLE = 0,
	MAGIC = 1
};

enum class SpellTargetRestriction: sf::Uint32 {
	SELF= (1 << 0),
	FRIENDLY = (1 << 1),
	ENEMY = (1 << 2),
};

enum class SpellCastResultCode: sf::Uint8 {
	SUCCESS = 0,
	OUT_OF_RANGE = 1,
	CANT_SEE_TARGET = 2,
	INTERRUPTED = 3,
	TARGET_NOT_EXISTS = 4,
	ALREADY_CASTING = 5,
	NOT_ENOUGH_MANA = 6,
	HAS_COOLDOWN = 7,
	TARGET_IS_DEAD = 8
};

inline std::string getStringSpellErrorByResultCode(SpellCastResultCode code) {
	switch (code) {
		case SpellCastResultCode::OUT_OF_RANGE:
			return "Target is out of range.";
		case SpellCastResultCode::ALREADY_CASTING:
			return "You are already casting another spell";
		case SpellCastResultCode::CANT_SEE_TARGET:
			return "You can not see target entity";
		case SpellCastResultCode::INTERRUPTED:
			return "Your spell was interrupted";
		case SpellCastResultCode::NOT_ENOUGH_MANA:
			return "You do not have enough mana";
		case SpellCastResultCode::TARGET_NOT_EXISTS:
			return "Your target do not exists";
		case SpellCastResultCode::HAS_COOLDOWN:
			return "Spell is not ready yet";
		case SpellCastResultCode::TARGET_IS_DEAD:
			return "Your target is dead";
		default:
			return "Your spell has not been casted";
	}
}


#endif
