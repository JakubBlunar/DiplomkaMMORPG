#ifndef S_EFFECT_H
#define S_EFFECT_H

#include "Entity.h"
#include "JsonLoader.h"

namespace s {
	class Effect {
	protected:
		int id = -1;
		std::string name;
	public:
		int getId();
		void setId(int id) {
			this->id = id;
		}

		void setName(std::string name) {
			this->name = name;
		}


				
		virtual void loadFromJson(json jsonData);
		virtual void loadFromFile(std::string filename);

		virtual void apply(Entity* caster, Entity* target) = 0;;
		virtual Effect* clone() = 0;
	};
}


#endif
