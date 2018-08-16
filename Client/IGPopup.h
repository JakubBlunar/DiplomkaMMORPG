#pragma once
#include "IGWindow.h"
#include <functional>

class IGPopup :
	public IGWindow
{
	std::string title;
	std::string text;
	std::string buttonText;
	std::function<void()> onButtonClick;

public:
	IGPopup(std::string title,std::string text, sf::Vector2f size, std::string buttonText);
	IGPopup(std::string title,std::string text, sf::Vector2f size, std::string buttonText, std::function<void()> onButtonClick);
	~IGPopup();
protected:
	void render(Game* g, IGManager* manager) override;
	void beforeRender(Game* g) override;
};

