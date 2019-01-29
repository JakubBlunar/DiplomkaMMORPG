#ifndef CHAT_CONSTANTS_H
#define CHAT_CONSTANTS_H

#include "SFML/System.hpp"
#include <string>
#include "../Shared/EntityConstants.h"
#include <iomanip>
#include <sstream>

enum class MessageType : sf::Uint8 {
	SAY = 0,
	SERVER_ANNOUNCEMENT = 1,
	COMBAT_LOG = 2
};

class ChatUtils {
public:
	static std::string floatToString(float number, int precision) {
		std::stringstream stream;
		stream << std::fixed << std::setprecision(precision) << number;
		return stream.str();
	}


	static std::string formatLogForAttributeChange(EntityAttributeType attribute, std::string casterName,
	                                        std::string targetName, float change) {
		switch (attribute) {
			case EntityAttributeType::HP: {
				if (change > 0) {
					return casterName + " heals " + targetName + " for " + floatToString(change, 0);
				}
				return casterName + " hit " + targetName + " for " + floatToString(abs(change), 0) + " damage";
			}
			default:
				return "";
		}
	}
};


#endif
