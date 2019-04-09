#ifndef IG_LEARN_NEW_SPELL_H
#define IG_LEARN_NEW_SPELL_H

#include "IGWindow.h"

struct SpellInfo;

class IGLearnNewSpell :
	public IGWindow {
public:
	IGLearnNewSpell();
	virtual ~IGLearnNewSpell();

	void setSpells(std::vector<SpellInfo*>* spells) {
		delete this->spells;
		this->spells = spells;

	}

protected:
	void render(Game* g, IGManager* manager) override;
	void beforeRender(Game* game) override;

	std::vector<SpellInfo*>* spells;
	int chosenSpell;
};

#endif
