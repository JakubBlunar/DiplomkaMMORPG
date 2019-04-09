#include "PacketManager.h"
#include <SFML/Network/SocketSelector.hpp>
#include <iostream>
#include "EventId.h"
#include "Game.h"
#include "EventLoginResponse.h"
#include "EventDispatcher.h"
#include "EventMovementChange.h"
#include "EventCharacterMapLeave.h"
#include "EventNpcsMovementChange.h"
#include "EventAttributesChanged.h"
#include "EventCharacterChooseResponse.h"
#include "EventCharacterMapJoin.h"
#include "EventNpcMovementChange.h"
#include "EventNpcStatusChanged.h"
#include "EventSpellCastResult.h"
#include "EventSendMessage.h"
#include "EventFreeSpellToLearn.h"
#include "EventLearnSpell.h"
#include "EventNpcPositionChange.h"

PacketManager::PacketManager(Game* g) : game(nullptr), latencyCheckThread(nullptr) {
	game = g;
	ClientSettings* settings = ClientSettings::instance();
	if (socket.connect(settings->host, static_cast<short>(settings->port)) != sf::Socket::Done) {
		game->print("An error ocurred connecting to server");
		connected = false;
	}
	else {
		connected = true;
		game->print("connected");
	}

	socket.setBlocking(false);
}


PacketManager::~PacketManager() {
	delete latencyCheckThread;
}

bool PacketManager::isConnected() const {
	return connected;
}

void PacketManager::latencyCheck() {
	while (game->isRunning() && connected) {
		int id = rand() % INT32_MAX;

		sf::Packet packet;
		packet << EventId::LATENCY << id;
		sendPacket(&packet);
		statistics.packetSend(id);
		sf::sleep(sf::seconds(5));
	}
}


void PacketManager::startRecieve() {
	sf::SocketSelector selector;
	selector.add(socket);

	sf::Packet packet;

	latencyCheckThread = new sf::Thread(&PacketManager::latencyCheck, this);
	latencyCheckThread->launch();

	while (game->isRunning() && connected) {
		if (selector.wait()) {
			socketMutex.lock();
			if (socket.receive(packet) == sf::Socket::Disconnected) {
				connected = false;
				game->print("Disconected from server");
				break;
			}

			socketMutex.unlock();

			int packetType;
			GameEvent* e;
			if (packet >> packetType) {
				//const auto duration = statistics.packetRecieve(id);

				EventId pt = static_cast<EventId>(packetType);
				switch (pt) {
					case EventId::LATENCY:
						int id;
						if (packet >> id) {
							statistics.packetRecieve(id);
						}
						break;
					case LOGINRESPONSE:
						e = new EventLoginResponse();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventLoginResponse>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					case CHARACTER_CHOOSE_RESPONSE:
						e = new EventCharacterChooseResponse();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventCharacterChooseResponse>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					case CHARACTER_MAP_JOIN:
						e = new EventCharacterMapJoin();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventCharacterMapJoin>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					case CHARACTER_MAP_LEAVE:
						e = new EventCharacterMapLeave();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventCharacterMapLeave>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					case MOVEMENT:
						e = new EventMovementChange();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventMovementChange>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					case NPC_MOVEMENT_CHANGE:
						e = new EventNpcMovementChange();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventNpcMovementChange>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					case NPCS_MOVEMENT_CHANGE:
						e = new EventNpcsMovementChange();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventNpcsMovementChange>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					case ATTRIBUTES_CHANGED:
						e = new EventAttributesChanged();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventAttributesChanged>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					case NPC_STATUS_CHANGED:
						e = new EventNpcStatusChanged();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventNpcStatusChanged>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					case SPELL_CAST_RESULT:
						e = new EventSpellCastResult();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventSpellCastResult>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					case SEND_MESSAGE: {
						e = new EventSendMessage();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventSendMessage>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					}
					case FREE_SPELL_TO_LEARN: {
						e = new EventFreeSpellToLearn();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventFreeSpellToLearn>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					}
					case LEARN_SPELL: {
						e = new EventLearnSpell();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventLearnSpell>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					}
					case NPC_POSITION_CHANGE: {
						e = new EventNpcPositionChange();
						if (e->loadFromPacket(&packet)) {
							EventDispatcher<EventNpcPositionChange>::dispatchEvent(e);
						}
						else {
							game->print("Error while parsing packet " + std::to_string(pt));
						}
						break;
					}
					default:
						game->print("Unknown packet type " + std::to_string(pt));
						break;
				}
			}

		}
	}

	latencyCheckThread->wait();
}

void PacketManager::sendPacket(sf::Packet* packet) {
	if (!connected)
		return;

	socketMutex.lock();
	socket.send(*packet);
	socketMutex.unlock();
}
