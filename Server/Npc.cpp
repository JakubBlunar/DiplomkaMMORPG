#include "Npc.h"
#include "JsonLoader.h"
#include "ServerGlobals.h"
#include "EventNpcMovementChange.h"

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

	return copy;
}

json s::Npc::toJson() const {
	json json;

	json["spawnId"] = spawnId;
	json["name"] = name;
	json["type"] = type;
	json["movementX"] = movement.x;
	json["movementY"] = movement.y;
	json["positionX"] = position.x;
	json["positionY"] = position.y;
	json["speed"] = speed;

	json["state"] = static_cast<int>(state);

	if (map) {
		json["mapId"] = map->getId();
	}

	return json;
}

void s::Npc::setName(string name) {
	this->name = name;
}

string s::Npc::getName() const {
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
