#include "NpcEvent.h"



s::NpcEvent::NpcEvent()
{

}


s::NpcEvent::~NpcEvent()
{

}

bool s::NpcEvent::loadFromPacket(sf::Packet * p)
{
	throw "Npc events cant load from packet";
}

sf::Packet* s::NpcEvent::toPacket() {
	throw "Npc event cant convert to packet";
}
