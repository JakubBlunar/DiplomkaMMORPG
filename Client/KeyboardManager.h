#ifndef KEYBOARD_MANAGER_H
#define KEYBOARD_MANAGER_H

#include "SFML\Graphics.hpp"
#include <map>

class KeyboardManager
{
	std::map<sf::Keyboard::Key, bool> keys;
public:
	KeyboardManager();
	~KeyboardManager();

	void handlePlayerInput(sf::Keyboard::Key, bool isPressed);
	bool isKeyPressed(sf::Keyboard::Key key);

};


#endif // !KEYBOARD_MANAGER_H