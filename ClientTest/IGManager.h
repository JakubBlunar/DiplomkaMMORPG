#ifndef IMGUIMANAGER_H
#define IMGUIMANAGER_H
#include <string>
#include <map>

class IGWindow;
class Game;

class IGManager
{
public:
	IGManager(Game* g);
	~IGManager();

	void addWindow(std::string s, IGWindow* w);
	IGWindow* getWindow(std::string s);
	void drawAll();

	void Open(std::string n);
	void OpenAll();

	bool AnyWindowFocused();

private:
	Game * game;
	std::map<std::string, IGWindow*> windows;

	static void IGManager::IGManagerSetupImGuiStyle(bool bStyleDark_, float alpha_);
};

#endif