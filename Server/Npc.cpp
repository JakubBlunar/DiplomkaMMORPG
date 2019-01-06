#include "Npc.h"
#include "JsonLoader.h"
#include "ServerGlobals.h"
#include "EventNpcMovementChange.h"
#include <spdlog/spdlog.h>
#include "TextFileLoader.h"

s::Npc::Npc(): command(nullptr) {
	body = nullptr;
	name = "";
	spawnId = -1;
	speed = 0;
	type = 0;
	size = sf::Vector2i(0, 0);
	movement = sf::Vector2f(0, 0);
	map = nullptr;
	deadTimestamp = sf::Time::Zero;
	state = NpcState::IDLE;
	spawnPosition = sf::Vector2f(-1,-1);

	entityType = EntityType::NPC;

	luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::io, sol::lib::string, sol::lib::os, sol::lib::math);
	luaState["npc"] = this;
	// make usertype metatable
    luaState.new_usertype<Npc>("Npc",
        "name", &Npc::name,
        "speed", &Npc::speed,
		"spawnId", &Npc::spawnId,
		"getAttribute", &Npc::getAttribute,
		"getState", &Npc::getNpcState
    );

	luaState.script("math.randomseed(os.time())");
}


s::Npc::~Npc() {
	
}

s::NpcCommand* s::Npc::getNpcCommand() {
	sf::Lock lock(mutex);
	return command;
}
void s::Npc::setNpcCommand(NpcCommand* command) {
	sf::Lock lock(mutex);
	this->command = command;
}

void s::Npc::setSpawnId(int id)
{
	this->spawnId = id;
}

int s::Npc::getSpawnId() const {
	return spawnId;
}

void s::Npc::loadFromJson(std::string file)
{
	json jsonData = JsonLoader::instance()->loadJson(file);

	name = jsonData["name"].get<json::string_t>();
	type = (int) jsonData["type"].get<json::number_integer_t>();
	speed = (float) jsonData["speed"].get<json::number_float_t>();

	respawnTime = sf::seconds((float) jsonData["respawnTime"].get<json::number_float_t>());
	deadTimestamp = sf::Time::Zero;

	std::string renderFile = jsonData["render"].get<json::string_t>();
	json renderData = JsonLoader::instance()->loadJson("Graphics/Npcs/" + renderFile);

	int width = (int)renderData["width"].get<json::number_integer_t>();

	size = sf::Vector2i(width, width);
	movement = sf::Vector2f(0, 0);

	json attributes = jsonData["attributes"].get<json::array_t>();
	int index = 0;
	for (json::iterator it = attributes.begin(); it != attributes.end(); ++it) {
		setAttributeByIndex(index, *it);
		index++;
	}

	auto exists = jsonData.find("script");
    if (exists == jsonData.end()) {
		npc_script = TextFileLoader::instance()->loadFile("Npcs/Scripts/dummy_npc.lua");
    } else {
	    std::string scriptFile = jsonData["script"].get<json::string_t>();
		npc_script = TextFileLoader::instance()->loadFile("Npcs/Scripts/"+ scriptFile +".lua");
    }

	sol::load_result loadResult = luaState.load(npc_script);
	if (!loadResult.valid()) {
		sol::error err = loadResult;
		sol::load_status status = loadResult.status();
		spdlog::get("log")->error("Cannot load lua script from npc: {} , ERROR: {}", sol::to_string(status), err.what());
		throw "cannot parse lua";
	}
}

s::Npc* s::Npc::clone() const {
	Npc* copy = new Npc();

	copy->setSize(size);
	copy->setMovement(0, 0);
	copy->setType(type);
	copy->setName(name);
	copy->setSpeed(speed);
	copy->setBody(nullptr);
	copy->setMap(nullptr);

	copy->setRespawnTime(respawnTime);
	copy->npc_script = npc_script;

	int count = attributes.size();
	for (int i = 0; i < count; i++) {
		copy->setAttributeByIndex(i, attributes[i]);
	}

	copy->setSpawnPosition(spawnPosition);
	return copy;
}

json s::Npc::toJson() const {
	json jsonData;

	jsonData["spawnId"] = spawnId;
	jsonData["name"] = name;
	jsonData["type"] = type;
	jsonData["movementX"] = movement.x;
	jsonData["movementY"] = movement.y;
	jsonData["positionX"] = position.x;
	jsonData["positionY"] = position.y;
	jsonData["speed"] = speed;

	jsonData["state"] = static_cast<int>(state);
	jsonData["attributes"] = json(attributes);
	if (map) {
		jsonData["mapId"] = map->getId();
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
	if (lastMovement == movement) {
		return;
	}

	this->lastMovement = this->movement;
	this->movement = sf::Vector2f(movementX, movementY);
}

void s::Npc::setMovement(sf::Vector2f movement, NpcUpdateEvents * npcUpdateEvents) {
	if (lastMovement == movement) {
		return;
	}

	if (state == NpcState::DEAD) {
		movement = sf::Vector2f(0, 0);
	}

	this->lastMovement = this->movement;
	this->movement = movement;

	if (body) {
		body->SetLinearVelocity(b2Vec2(movement.x * PIXTOMET, movement.y * PIXTOMET));

		if (map) {
			if (!npcUpdateEvents) {
				EventNpcMovementChange e;
				e.spawnId = spawnId;
				e.speed = speed;
				e.velX = this->movement.x;
				e.velY = this->movement.y;
				e.x = position.x;
				e.y = position.y;

				map->sendEventToAllPlayers(&e);
			} else {
				if (!npcUpdateEvents->npcsMovementChange) {
					npcUpdateEvents->npcsMovementChange = new EventNpcsMovementChange();
				}
				npcUpdateEvents->npcsMovementChange->addNpcInfo(spawnId, position.x, position.y, this->movement.x, this->movement.y);
			}
		}
	}
}

sf::Vector2f s::Npc::getMovement() const {
	return movement;
}

void s::Npc::setType(int type) {
	this->type = type;
}

int s::Npc::getType() const
{
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

bool s::Npc::hasSpawnPosition() const
{
	return spawnPosition.x < 0;
}

void s::Npc::setSpawnPosition(sf::Vector2f position) {
	spawnPosition = position;
}

sf::Vector2f s::Npc::getSpawnPosition() const {
	return spawnPosition;
}

void s::Npc::setMovementDirection(sf::Vector2f direction, float speed, NpcUpdateEvents * npcUpdateEvents) {
	this->speed = speed;
	this->setMovement(sf::Vector2f(direction.x * speed, direction.y * speed), npcUpdateEvents);
}

void s::Npc::lock() {
	mutex.lock();
}

void s::Npc::unlock() {
	mutex.unlock();
}
