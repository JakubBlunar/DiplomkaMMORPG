#ifndef IG_CONSOLE_H
#define IG_CONSOLE_H

#include <imgui.h>
#include <locale>
#include "IGWindow.h"

class IGConsole: public IGWindow {
public:
	IGConsole();

	~IGConsole();

	char InputBuf[256];
	ImVector<char*> Items;
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
		for (int i = 0; i < Items.Size; i++)
			free(Items[i]);
		Items.clear();
		ScrollToBottom = true;
	}

	void AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		// FIXME-OPT
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		Items.push_back(Strdup(buf));
		ScrollToBottom = true;
	}

	void ExecCommand(const char* command_line);

	static int TextEditCallbackStub(ImGuiTextEditCallbackData* data)
	// In C++11 you are better off using lambdas for this sort of forwarding callbacks
	{
		IGConsole* console = (IGConsole*)data->UserData;
		return console->TextEditCallback(data);
	}

	int TextEditCallback(ImGuiTextEditCallbackData* data);
	

protected:
	void render(Game* g, IGManager* manager) override;
};

#endif
