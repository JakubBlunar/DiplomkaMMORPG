#include "KeyboardManager.h"



KeyboardManager::KeyboardManager()
{
}


KeyboardManager::~KeyboardManager()
{
}

void KeyboardManager::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	keys[key] = isPressed;
}

bool KeyboardManager::isKeyPressed(sf::Keyboard::Key key)
{
	auto search = keys.find(key);
	if (search != keys.end()) {
		return search->second;
	}
	return false;
}
