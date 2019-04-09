#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include "EventLoginRequest.h"
#include "Manager.h"
#include "EventCharacterChoose.h"
#include "EventCharacterLogout.h"

namespace s {
	class Session;
}

namespace s {

	class AuthManager : public Manager {
	public:
		AuthManager();
		~AuthManager();

		void handleEvent(EventLoginRequest* event, s::Session* playerSession, s::Server* s);
		void handleEvent(EventCharacterChoose* event, s::Session* playerSession, s::Server* s);
		void handleEvent(EventCharacterLogout* event, s::Session* playerSession, s::Server* s);
	};

}
#endif
