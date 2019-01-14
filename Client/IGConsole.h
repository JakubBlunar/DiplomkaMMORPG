#ifndef IG_CONSOLE_H
#define IG_CONSOLE_H

#include <imgui.h>
#include <locale>
#include "IGWindow.h"
#include "Subscriber.h"
#include "EventSendMessage.h"

struct ConsoleItem {
	std::string message;
	time_t time;
	MessageType type;
	std::string player;
	int playerId;
};

class IGConsole: public IGWindow, Subscriber {
public:
	Game* game;

	IGConsole();
	~IGConsole();

	void setGame(Game* g) {
		this->game = g;
	}

	std::vector<ConsoleItem*> messages;

	char InputBuf[256];
	bool ScrollToBottom;
	ImVector<char*> History;
	int HistoryPos; // -1: new line, 0..History.Size-1 browsing history.
	ImVector<const char*> Commands;

	bool reclaim_focus = false;
	bool remove_focus = false;

	void reclaimFocus() {
		reclaim_focus = true;
	}

	static int Stricmp(const char* str1, const char* str2) {
		int d;
		while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
			str1++;
			str2++;
		}
		return d;
	}

	static int Strnicmp(const char* str1, const char* str2, int n) {
		int d = 0;
		while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
			str1++;
			str2++;
			n--;
		}
		return d;
	}

	static char* Strdup(const char* str) {
		size_t len = strlen(str) + 1;
		void* buff = malloc(len);
		return (char*)memcpy(buff, (const void*)str, len);
	}

	static void Strtrim(char* str) {
		char* str_end = str + strlen(str);
		while (str_end > str && str_end[-1] == ' ') str_end--;
		*str_end = 0;
	}

	void ClearLog() {
		for (unsigned int i = 0; i < messages.size(); i++)
			delete(messages[i]);
		messages.clear();
		ScrollToBottom = true;
	}

	void AddLog(ConsoleItem *item) IM_FMTARGS(2)
	{
		int count = messages.size();

		if (messages.empty()) {
			messages.push_back(item);
			ScrollToBottom = true;
			return;
		}

		int i = count - 1;
		if (messages[i]->time <= item->time) {
			messages.push_back(item);
			ScrollToBottom = true;
			return;
		}

		
		while (item->time < messages[i]->time && i > 0) {
			i--;
		}


		messages.insert(messages.begin() + i, item);
		ScrollToBottom = true;
	}

	void ExecCommand(const char* command_line);

	static int TextEditCallbackStub(ImGuiTextEditCallbackData* data)
	
	{
		IGConsole* console = (IGConsole*)data->UserData;
		return console->TextEditCallback(data);
	}

	int TextEditCallback(ImGuiTextEditCallbackData* data);
	

protected:
	void render(Game* g, IGManager* manager) override;
public:
	void handleEvent(GameEvent* event) override;
};

#endif
