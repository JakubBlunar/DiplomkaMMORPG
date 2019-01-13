#ifndef S_CHAT_MANAGER_H
#define S_CHAT_MANAGER_H

#include "Manager.h"
#include "EventSendMessage.h"

class EventCharacterChoose;

namespace s {
	class Session;

	class ChatManager :
		public Manager {
	public:
		ChatManager();
		virtual ~ChatManager();

		void handleEvent(EventCharacterChoose* event, s::Session* playerSession, s::Server* s) const;
		void handleEvent(EventSendMessage* event, s::Session* playerSession, s::Server* s);
	};

}

#endif