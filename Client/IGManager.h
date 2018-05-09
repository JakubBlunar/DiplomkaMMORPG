#ifndef IMGUIMANAGER_H
#define IMGUIMANAGER_H
#include <string>
#include <map>

class IGWindow;
class Game;

class IGManager
{
public:
	IGManager();
	~IGManager();

	void addWindow(std::string s, IGWindow* w);
	IGWindow* getWindow(std::string s);
	void drawAll(Game* g);

	void Open(std::string n);
	void close(std::string n);
	void OpenAll();

	bool isVisible(std::string n);
	bool AnyWindowFocused();

private:
	std::map<std::string, IGWindow*> windows;

	static void IGManagerSetupImGuiStyle(bool bStyleDark_, float alpha_);
};

#endif