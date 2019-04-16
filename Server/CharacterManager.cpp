#include "CharacterManager.h"
#include "Character.h"
#include "Session.h"
#include "Account.h"
#include "EventAttributesChanged.h"
#include "Map.h"
#include "Npc.h"
#include "Utils.h"
#include "EventIncreaseCharacterAttribute.h"
#include "EventLearnSpell.h"
#include "EventFreeSpellToLearn.h"
#include "Database.h"
#include "NpcEventNpcIsIdle.h"
#include "EventDispatcher.h"
#include "EventNpcStatusChanged.h"
#include "EventCharacterPositionChanged.h"

s::CharacterManager::CharacterManager() : server(nullptr) {}

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

void s::CharacterManager::handleEvent(EventLearnSpell* event, Session* playerSession, Server* s) const {
	SpellHolder* spellHolder = SpellHolder::instance();

	try {
		SpellInfo* si = spellHolder->getSpellInfo(event->spellId);
		Character* character = playerSession->getAccount()->getCharacter();

		if (character->spells.getSpell(si->id) != nullptr) {
			return;
		}

		character->spells.addAvailableSpell(si);

		EventLearnSpell e;
		e.spellId = si->id;
		e.success = true;

		sf::Packet* p = e.toPacket();
		playerSession->sendPacket(p);
		delete p;

		std::string query = "INSERT INTO character_spells(`spellType`,`characterId` ,`createdAt` ,`updatedAt`) VALUES(";
		query.append(std::to_string(si->id) + ", ");
		query.append(std::to_string(character->id) + ", ");
		query.append(" NOW(), NOW());");

		bool success = Database::i()->executeModify(query) > 0;
		if (!success) {
			throw "Cannot save into database";
		}

		std::vector<SpellInfo*>* spellsToChoice = getFreeSpellsForLearn(character);
		if (spellsToChoice && !spellsToChoice->empty()) {
			EventFreeSpellToLearn e;
			for (std::vector<SpellInfo*>::iterator it = spellsToChoice->begin(); it != spellsToChoice->end(); ++it) {
				SpellInfo* si = *it;
				e.spellIds.push_back(si->id);
			}

			sf::Packet* p = e.toPacket();
			playerSession->sendPacket(p);
			delete p;
		}
	}
	catch (...) {
		EventLearnSpell e;
		e.spellId = event->spellId;
		e.success = false;

		sf::Packet* p = e.toPacket();
		playerSession->sendPacket(p);
	}

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

	Session* playerSession = character->getAccount()->getSession();

	float actualLevel = character->attributes.getAttribute(EntityAttributeType::LEVEL, true);
	if (actualLevel > characterLevel) {
		eventAttributesChanged->setChange(EntityAttributeType::LEVEL, actualLevel);

		float freeAttributes = character->attributes.getAttribute(EntityAttributeType::FREE_ATTRIBUTES, false);
		freeAttributes += server->serverSettings->freeAttributesAfterLevelUp;
		character->attributes.setAttribute(EntityAttributeType::FREE_ATTRIBUTES, freeAttributes);
		eventAttributesChanged->setChange(EntityAttributeType::FREE_ATTRIBUTES, freeAttributes);

		character->position.getMap()->sendEventToAllPlayers(eventAttributesChanged);

		log += "\n Congratulation! You have got new level!";

		std::vector<SpellInfo*>* spellsToChoice = getFreeSpellsForLearn(character);
		if (spellsToChoice && !spellsToChoice->empty()) {
			EventFreeSpellToLearn e;
			for (std::vector<SpellInfo*>::iterator it = spellsToChoice->begin(); it != spellsToChoice->end(); ++it) {
				SpellInfo* si = *it;
				e.spellIds.push_back(si->id);
			}

			sf::Packet* p = e.toPacket();
			playerSession->sendPacket(p);
			delete p;
		}
	}
	else {
		sf::Packet* p = eventAttributesChanged->toPacket();
		playerSession->sendPacket(p);
		delete p;
	}

	EventSendMessage logEvent;
	logEvent.message = log;
	logEvent.messageType = MessageType::COMBAT_LOG;
	logEvent.playerId = -1;
	logEvent.time = Utils::getActualUtcTime();

	sf::Packet* p = logEvent.toPacket();
	playerSession->sendPacket(p);

	delete eventAttributesChanged;
}

void s::CharacterManager::characterDied(Character * character, Npc* npc)
{
	for (Npc* npc : character->combat.attackingNpcs) {
		server->npcManager.removeCombat(npc, character);
	}
	character->combat.reset();


	EventSendMessage mess;
	mess.messageType = MessageType::COMBAT_LOG;
	mess.message = "You have died.";
	mess.time = Utils::getActualUtcTime();

	sf::Packet* p = mess.toPacket();
	character->getAccount()->getSession()->sendPacket(p);
	delete p;

	Map* map = character->position.getMap();

	float hp = character->attributes.getAttribute(EntityAttributeType::BASE_HP, true);
	float mp = character->attributes.getAttribute(EntityAttributeType::BASE_MP, true);
	character->attributes.setAttribute(EntityAttributeType::HP, hp);
	character->attributes.setAttribute(EntityAttributeType::MP, mp);


	EventAttributesChanged che;
	che.entityType = EntityCategory::PLAYER;
	che.spawnId = character->id;
	che.setChange(EntityAttributeType::HP, hp);
	che.setChange(EntityAttributeType::MP, mp);

	map->sendEventToAllPlayers(&che);

	map->lock.lock();
	character->position.setPosition(map->respawn);
	map->lock.unlock();

	EventCharacterPositionChanged e;
	e.characterId = character->id;
	e.positionX = map->respawn.x;
	e.positionY = map->respawn.y;
	map->sendEventToAllPlayers(&e);

}

std::vector<s::SpellInfo*>* s::CharacterManager::getFreeSpellsForLearn(Character* character) const {
	SpellHolder* spellHolder = SpellHolder::instance();

	int level = (int)character->attributes.getAttribute(EntityAttributeType::LEVEL, false);
	for (int i = 1; i <= level; i++) {
		std::vector<SpellInfo*>* spells = spellHolder->getSpellsWithLevel(level);
		if (!spells || spells->empty()) continue;

		bool found = false;
		for (auto spellInfo : character->spells.availableSpells) {
			if (spellInfo.second->levelNeeded == level) {
				found = true;
				break;
			}
		}
		if (found) continue;
		return spells;
	}

	return nullptr;
}
