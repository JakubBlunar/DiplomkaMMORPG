#include "CharacterManager.h"
#include "Character.h"
#include "Session.h"
#include "Account.h"
#include "EventAttributesChanged.h"
#include "Map.h"
#include "Npc.h"
#include "Utils.h"


s::CharacterManager::CharacterManager(): server(nullptr) {}

s::CharacterManager::~CharacterManager() {}

void s::CharacterManager::init(Server* server) {
	this->server = server;
}

void s::CharacterManager::handleEvent(EventIncreaseCharacterAttribute* event, Session* playerSession, Server* s) const {
	Character* character = playerSession->getAccount()->getCharacter();


	float freeAttributes = character->attributes.getAttribute(EntityAttributeType::FREE_ATTRIBUTES, false);
	if (freeAttributes <= 0) return;
	freeAttributes--;

	Map* map = character->position.getMap();
	EntityAttributeType attribute = event->attribute;
	float attributeChange = character->attributes.getAttribute(attribute, false);
	attributeChange++;

	character->attributes.setAttribute(attribute, attributeChange);
	character->attributes.setAttribute(EntityAttributeType::FREE_ATTRIBUTES, freeAttributes);

	EventAttributesChanged* changeEvent = new EventAttributesChanged();
	changeEvent->spawnId = character->id;
	changeEvent->entityType = PLAYER;

	changeEvent->setChange(EntityAttributeType::FREE_ATTRIBUTES, freeAttributes);
	changeEvent->setChange(attribute, attributeChange);

	switch (attribute) {
		case EntityAttributeType::STAMINA: {
			character->attributes.recalcMaxHealth();
			changeEvent->setChange(EntityAttributeType::BASE_HP,
			                       character->attributes.getAttribute(EntityAttributeType::BASE_HP, true));
			break;
		}
		case EntityAttributeType::INTELECT: {
			character->attributes.recalcMaxMana();
			changeEvent->setChange(EntityAttributeType::BASE_MP,
			                       character->attributes.getAttribute(EntityAttributeType::BASE_MP, true));
			break;
		}
		default: break;
	}

	map->sendEventToAllPlayers(changeEvent);
}

void s::CharacterManager::handleNpcKill(Character* character, Npc* npc) const {
	character->combat.removeAttackingNpc(npc);

	float characterLevel = character->attributes.getAttribute(EntityAttributeType::LEVEL, true);
	float npcLevel = npc->attributes.getAttribute(EntityAttributeType::LEVEL, true);

	float experience = npc->getAttribute(EntityAttributeType::EXPERIENCE, true);
	if (npcLevel < characterLevel) {
		if (characterLevel - npcLevel < 3) {
			experience = ceil(experience * 0.5f);
		}
		else {
			experience = 0;
		}
	}
	else if (npcLevel > characterLevel) {
		experience = ceil(experience * 1.5f);
	}

	std::string log = npc->getName() + " died. ";

	experience *= server->serverSettings->xpRate;
	log += "You gained " + ChatUtils::floatToString(experience, 0) + "xp.";

	float characterExperience = character->attributes.getAttribute(EntityAttributeType::EXPERIENCE, true);
	float newExperience = characterExperience + experience;
	character->attributes.setAttribute(EntityAttributeType::EXPERIENCE, newExperience);
	character->attributes.recalcLevel();

	EventAttributesChanged* eventAttributesChanged = new EventAttributesChanged();
	eventAttributesChanged->spawnId = character->id;
	eventAttributesChanged->entityType = PLAYER;
	eventAttributesChanged->setChange(EntityAttributeType::EXPERIENCE, newExperience);

	float actualLevel = character->attributes.getAttribute(EntityAttributeType::LEVEL, true);
	if (actualLevel > characterLevel) {
		eventAttributesChanged->setChange(EntityAttributeType::LEVEL, actualLevel);

		float freeAttributes = character->attributes.getAttribute(EntityAttributeType::FREE_ATTRIBUTES, false);
		freeAttributes += server->serverSettings->freeAttributesAfterLevelUp;
		character->attributes.setAttribute(EntityAttributeType::FREE_ATTRIBUTES, freeAttributes);
		eventAttributesChanged->setChange(EntityAttributeType::FREE_ATTRIBUTES, freeAttributes);

		character->position.getMap()->sendEventToAllPlayers(eventAttributesChanged);

		log += "\n Congratulation! You have got new level!";
	}
	else {
		sf::Packet* p = eventAttributesChanged->toPacket();
		character->getAccount()->getSession()->sendPacket(p);
		delete p;
	}

	EventSendMessage logEvent;
	logEvent.message = log;
	logEvent.messageType = MessageType::COMBAT_LOG;
	logEvent.playerId = -1;
	logEvent.time = Utils::getActualUtcTime();
	
	sf::Packet* p = logEvent.toPacket();
	character->getAccount()->getSession()->sendPacket(p);

	delete eventAttributesChanged;
}
