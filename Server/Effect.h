#ifndef S_EFFECT_H
#define S_EFFECT_H

#include "Entity.h"
#include "JsonLoader.h"

namespace s {
	class Effect {
	protected:
		int id = -1;
	public:
		int getId();
				
		virtual void loadFromJson(json jsonData);
		virtual void loadFromFile(std::string filename);

		virtual void apply(Entity* caster, Entity* target) = 0;;
		virtual Effect* clone() = 0;
	};
}


#endif
