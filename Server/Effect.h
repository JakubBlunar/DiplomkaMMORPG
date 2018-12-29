#ifndef S_EFFECT_H
#define S_EFFECT_H
#include <list>
#include "Entity.h"
#include "JsonLoader.h"

namespace s {
	class Effect {
	protected:
		int id = -1;
		Entity* entity = nullptr;

		virtual void applyEffect(Entity* entity);
		virtual void removeEffect(Entity* entity);
	public:
		int getId();
		void apply(Entity* entity);
		void remove(Entity* entity);
		
		virtual void loadFromJson(json jsonData);
		virtual void loadFromFile(std::string filename);

		Effect* clone();
	};
}


#endif
