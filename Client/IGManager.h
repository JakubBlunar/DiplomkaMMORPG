#ifndef IMGUIMANAGER_H
#define IMGUIMANAGER_H
#include <string>
#include <map>
#include "IGPopup.h"
#include <queue>

class IGWindow;
class Game;

class IGManager
{
public:
	IGManager();
	~IGManager();

	void addWindow(std::string s, IGWindow* w);
	IGWindow* getWindow(std::string s);
	void drawAll(Game* game);

	void Open(std::string n);
	void close(std::string n);
	void OpenAll();

	bool isVisible(std::string n);
	bool AnyWindowFocused();

	void pushPopup(IGPopup* popup);
	bool isShowingPopup() const;
private:
	std::map<std::string, IGWindow*> windows;
	std::queue<IGPopup*> popups;

	static void IGManagerSetupImGuiStyle(bool bStyleDark_, float alpha_);
};

#endif