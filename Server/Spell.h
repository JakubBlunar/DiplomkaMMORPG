#ifndef S_SPELL_H
#define S_SPELL_H

#include "Entity.h"
#include "Effect.h"
#include "EntityAttributes.h"

namespace s {
	class Server;
	class SpellEvent;

	struct SpellInfo {
		std::string name;
		float manaCost;
		sf::Time cooldownTime;
		sf::Time castingTime;
		int id;
		sf::Time globalCooldownTime;
		float maxRange;
		EntityType spellCategory;
	};

	class Spell {
	protected:
		int instanceId;
		Entity* target;
		Entity* owner;
		std::list<Effect*> effects;
	public:
		Spell();
		virtual ~Spell();

		SpellInfo spellInfo;

		void addEffect(Effect* effect);

		virtual void cast(Entity* entity);

		virtual Spell* clone() const;

		void setInstanceId(int id) {
			this->instanceId = id;
		}
		int getInstanceId() const {
			return instanceId;
		}

		void setId(sf::Int32 id) {
			this->instanceId = id;
		}

		void setTarget(Entity* target) {
			this->target = target;
		}

		Entity* getTarget() {
			return target;
		}

		void setOwner(Entity* target) {
			this->target = target;
		}

		Entity* getOwner() {
			return target;
		};

		virtual void loadFromJson(json data);
	};

}


#endif
