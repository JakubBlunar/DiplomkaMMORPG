#include "Effect.h"

void s::Effect::applyEffect(Entity* entity) {}
void s::Effect::removeEffect(Entity* entity) {}
int s::Effect::getId() { return id; }
void s::Effect::apply(Entity* entity) {
	
}

void s::Effect::remove(Entity* entity) {}
void s::Effect::loadFromJson(json jsonData) {}
void s::Effect::loadFromFile(std::string filename) {}

s::Effect* s::Effect::clone() {
	return new Effect();
}
