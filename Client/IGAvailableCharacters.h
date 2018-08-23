#ifndef IG_AVAILABLE_CHARACTERS_H
#define IG_AVAILABLE_CHARACTERS_H

#include "IGWindow.h"

class IGAvailableCharacters :
	public IGWindow {
public:
	IGAvailableCharacters();
	~IGAvailableCharacters();
protected:
	void sendCharacterChoice(int id, Game* g) const;
	void render(Game* g, IGManager* manager) override;
};

#endif
