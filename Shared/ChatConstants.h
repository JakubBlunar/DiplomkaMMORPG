#ifndef CHAT_CONSTANTS_H
#define CHAT_CONSTANTS_H

#include "SFML/System.hpp"

enum class MessageType: sf::Uint8 {
	SAY = 0,
	SERVER_ANNOUNCEMENT = 1
};

#endif