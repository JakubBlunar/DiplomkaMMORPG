#include "KeyboardManager.h"


KeyboardManager::KeyboardManager() {
}


KeyboardManager::~KeyboardManager() {
}

void KeyboardManager::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	if (isPressed) {
		keys[key] = isPressed;
	}
	else {
		std::map<sf::Keyboard::Key, bool>::iterator iter = keys.find(key);
		if (iter != keys.end())
			keys.erase(iter);
	}
}

bool KeyboardManager::isKeyPressed(sf::Keyboard::Key key) {
	auto search = keys.find(key);
	if (search != keys.end()) {
		return search->second;
	}
	return false;
}
