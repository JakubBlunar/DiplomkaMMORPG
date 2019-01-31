#ifndef S_EFFECT_MODIFY_ATTRIBUTES
#define S_EFFECT_MODIFY_ATTRIBUTES

#include "Effect.h"

namespace s {
	class Npc;
	class Character;

	class EffectModifyAttributes :
		public Effect {

		std::map<EntityAttributeType, float> casterModify;
		std::map<EntityAttributeType, float> targetModify;

		Entity* caster;
		Entity* target;

		void modifyCharacterAttributes(Character* character, std::map<EntityAttributeType, float>* modifiers) const;
		void modifyNpcAttributes(Npc* npc, std::map<EntityAttributeType, float>* modifiers);
	public:
		EffectModifyAttributes(SpellInfo spellInfo);
		virtual ~EffectModifyAttributes();

		void apply(Entity* caster, Entity* target) override;
		Effect* clone() override;

		void addCasterModify(EntityAttributeType attribute, float value);
		void addTargetModify(EntityAttributeType attribute, float value);

		void loadFromJson(json jsonData) override;
		void loadFromFile(std::string filename) override;
	};

}

#endif
