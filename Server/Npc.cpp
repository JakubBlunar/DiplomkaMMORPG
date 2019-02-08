#include "Npc.h"
#include "JsonLoader.h"
#include "ServerGlobals.h"
#include "EventNpcMovementChange.h"
#include <spdlog/spdlog.h>
#include "TextFileLoader.h"
#include "EventNpcStatusChanged.h"
#include "NpcCommandCombat.h"

s::Npc::Npc(): command(nullptr), luaConnector(this) {
	name = "";
	spawnId = -1;
	type = 0;
	deadTimestamp = sf::Time::Zero;
	state = NpcState::IDLE;
	spawnPosition = sf::Vector2f(-1, -1);

	entityType = EntityType::NPC;

	luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::io, sol::lib::string, sol::lib::os,
	                        sol::lib::math);
	luaState.script("math.randomseed(os.time())");
}


s::Npc::~Npc() { }

s::NpcCommand* s::Npc::getNpcCommand() {
	return command;
}

void s::Npc::setNpcCommand(NpcCommand* command) {
	this->command = command;
}

void s::Npc::setSpawnId(int id) {
	this->spawnId = id;
}

int s::Npc::getSpawnId() const {
	return spawnId;
}

void s::Npc::loadFromJson(std::string file) {
	json jsonData = JsonLoader::instance()->loadJson(file);

	name = jsonData["name"].get<json::string_t>();
	type = (int)jsonData["type"].get<json::number_integer_t>();

	respawnTime = sf::seconds((float)jsonData["respawnTime"].get<json::number_float_t>());
	deadTimestamp = sf::Time::Zero;

	std::string renderFile = jsonData["render"].get<json::string_t>();
	json renderData = JsonLoader::instance()->loadJson("Graphics/Npcs/" + renderFile);

	int width = (int)renderData["width"].get<json::number_integer_t>();

	position.setSize(sf::Vector2i(width, width));
	position.setMovement(sf::Vector2f(0, 0));

	json attributes = jsonData["attributes"].get<json::array_t>();
	int index = 0;
	for (json::iterator it = attributes.begin(); it != attributes.end(); ++it) {
		this->attributes.setAttributeByIndex(index, *it);
		index++;
	}


	this->attributes.recalcArmor();
	this->attributes.recalcResistance();
	this->attributes.recalcDodgeChance();

	npc_script = TextFileLoader::instance()->loadFile("Npcs/Scripts/common.lua");
	auto exists = jsonData.find("script");
	if (exists == jsonData.end()) {
		npc_script += " \n" + TextFileLoader::instance()->loadFile("Npcs/Scripts/default_creature.lua");
	}
	else {
		std::string scriptFile = jsonData["script"].get<json::string_t>();
		npc_script += " \n" + TextFileLoader::instance()->loadFile("Npcs/Scripts/" + scriptFile + ".lua");
	}

	SpellInfo* melleAttack = SpellHolder::instance()->getSpellInfo(2);
	spells.addAvailableSpell(melleAttack);

	if (jsonData.find("spells") != jsonData.end()) {
		json jSpells = jsonData["spells"].get<json::array_t>();

		for (json::iterator it = jSpells.begin(); it != jSpells.end(); ++it) {
			SpellInfo* spellInfo = SpellHolder::instance()->getSpellInfo(*it);
			spells.addAvailableSpell(spellInfo);
		}
	}

	luaConnector.connect();
}

s::Npc* s::Npc::clone() {
	Npc* copy = new Npc();

	copy->position.setSize(position.getSize());
	copy->setMovement(0, 0);
	copy->setType(type);
	copy->setName(name);
	copy->position.setBody(nullptr);
	copy->position.setMap(nullptr);

	copy->setRespawnTime(respawnTime);
	copy->npc_script = npc_script;

	int count = attributes.getCount();
	for (int i = 0; i < count; i++) {
		copy->attributes.setAttributeByIndex(i, attributes.getAttributeByIndex(i));
	}

	copy->setSpawnPosition(spawnPosition);

	std::map<int, SpellInfo*>* availableSpells = spells.getAvailableSpells();
	for (std::map<int, SpellInfo*>::iterator it = availableSpells->begin(); it != availableSpells->end(); ++it) {
		copy->spells.addAvailableSpell(it->second);
	}

	copy->luaConnector.connect();
	return copy;
}

json s::Npc::toJson() {
	json jsonData;

	sf::Vector2f actualPosition = position.getPosition();
	sf::Vector2f actualMovement = position.getMovement();

	jsonData["spawnId"] = spawnId;
	jsonData["name"] = name;
	jsonData["type"] = type;
	jsonData["movementX"] = actualMovement.x;
	jsonData["movementY"] = actualMovement.y;
	jsonData["positionX"] = actualPosition.x;
	jsonData["positionY"] = actualPosition.y;

	jsonData["state"] = static_cast<int>(state);
	jsonData["attributes"] = json::array();

	int attributesCount = attributes.getCount();
	for (int i = 0; i < attributesCount; i++) {
		jsonData["attributes"].push_back(attributes.getAttributeByIndex(i));
	}

	if (position.getMap()) {
		jsonData["mapId"] = position.getMap()->getId();
	}

	std::map<int, SpellInfo*>* availableSpells = spells.getAvailableSpells();
	for (std::map<int, SpellInfo*>::iterator it = availableSpells->begin(); it != availableSpells->end(); ++it) {
		jsonData["spells"].push_back(it->second->id);
	}

	return jsonData;
}

void s::Npc::setName(std::string name) {
	this->name = name;
}

std::string s::Npc::getName() const {
	return name;
}

void s::Npc::setNpcState(NpcState state) {
	this->state = state;
}

NpcState s::Npc::getNpcState() const {
	return state;
}

void s::Npc::setMovement(float movementX, float movementY) {
	if (position.getLastMovement() == position.getMovement()) {
		return;
	}

	position.setLastMovement(this->position.movement);
	position.setMovement(sf::Vector2f(movementX, movementY));
}

void s::Npc::setMovement(sf::Vector2f movement, NpcUpdateEvents* npcUpdateEvents) {
	if (position.getLastMovement() == movement) {
		return;
	}

	if (state == NpcState::DEAD) {
		movement = sf::Vector2f(0, 0);
	}

	position.setLastMovement(position.getMovement());
	position.setMovement(movement);

	b2Body* body = position.getBody();
	Map* map = position.getMap();
	if (body) {
		body->SetLinearVelocity(b2Vec2(movement.x * PIXTOMET, movement.y * PIXTOMET));

		if (map) {
			sf::Vector2f actualPosition = position.getPosition();
			sf::Vector2f actualMovement = position.getMovement();

			if (!npcUpdateEvents) {
				EventNpcMovementChange e;
				e.spawnId = spawnId;
				e.speed = attributes.getAttribute(EntityAttributeType::SPEED, true);
				e.velX = actualMovement.x;
				e.velY = actualMovement.y;
				e.x = actualPosition.x;
				e.y = actualPosition.y;

				map->sendEventToAllPlayers(&e);
			}
			else {
				if (!npcUpdateEvents->npcsMovementChange) {
					npcUpdateEvents->npcsMovementChange = new EventNpcsMovementChange();
				}
				npcUpdateEvents->npcsMovementChange->addNpcInfo(spawnId, actualPosition.x, actualPosition.y,
				                                                actualMovement.x, actualMovement.y);
			}
		}
	}
}

sf::Vector2f s::Npc::getMovement() const {
	return position.getMovement();
}

void s::Npc::setType(int type) {
	this->type = type;
}

int s::Npc::getType() const {
	return type;
}

sf::Time s::Npc::getRespawnTime() const {
	return respawnTime;
}

void s::Npc::setRespawnTime(sf::Time respawnTime) {
	this->respawnTime = respawnTime;
}

void s::Npc::setDeadTimestamp(sf::Time deadTimestamp) {
	this->deadTimestamp = deadTimestamp;
}

sf::Time s::Npc::getDeadTimestamp() const {
	return deadTimestamp;
}

bool s::Npc::isAlive() const {
	return state != NpcState::DEAD;
}

bool s::Npc::hasSpawnPosition() const {
	return spawnPosition.x > 0;
}

void s::Npc::setSpawnPosition(sf::Vector2f position) {
	spawnPosition = position;
}

sf::Vector2f s::Npc::getSpawnPosition() const {
	return spawnPosition;
}

void s::Npc::setMovementDirection(sf::Vector2f direction, float speed, NpcUpdateEvents* npcUpdateEvents) {
	this->attributes.setAttribute(EntityAttributeType::SPEED, speed);
	this->setMovement(sf::Vector2f(direction.x * speed, direction.y * speed), npcUpdateEvents);
}

float s::Npc::getAttribute(EntityAttributeType attribute, bool withBonus) {
	return attributes.getAttribute(attribute, withBonus);
}

b2Body* s::Npc::getBody() const {
	return position.getBody();
}

void s::Npc::startCombat(Character* character) {


	EventNpcStatusChanged* statusChangedEvent = new EventNpcStatusChanged();
	statusChangedEvent->npcState = NpcState::COMBAT;
	statusChangedEvent->spawnId = spawnId;

	combat.startCombatPosition = position.getPosition();
	combat.target = character;
	combat.setAttackingCharacter(character);

	character->combat.setAttackingNpc(this);

	NpcCommand* c = command;
	setNpcCommand(new NpcCommandCombat(this, server));

	state = NpcState::COMBAT;
	position.getMap()->sendEventToAllPlayers(statusChangedEvent);
	delete statusChangedEvent;
	delete c;
}

sf::Vector2f s::Npc::getPosition() const {
	return position.getPosition();
}

void s::Npc::lock() {
	mutex.lock();
}

void s::Npc::unlock() {
	mutex.unlock();
}

