#include "IGWindow.h"
#include <imgui.h>


IGWindow::IGWindow() : prevAlpha(0)
{
	this->visible = false;
	this->alpha = ImGui::GetStyle().Alpha;
}

IGWindow::IGWindow(const bool visible = false, const float alpha = ImGui::GetStyle().Alpha) : prevAlpha(0)
{
	this->visible = visible;
	this->alpha = alpha;
}


IGWindow::~IGWindow()
{
}


void IGWindow::draw(Game* g, IGManager* manager)
{
	if (!visible)
	{
		return;
	}

	beforeRender(g);
	render(g, manager);
	afterRender(g);
}

void IGWindow::open()
{
	visible = true;
}

void IGWindow::close()
{
	visible = false;
}

bool IGWindow::isOpened() const
{
	return visible;
}

bool IGWindow::isFocused() const
{
	if (!visible)
		return false;

	return focused;
}

float IGWindow::getAlpha() const
{
	return alpha;
}

void IGWindow::setAlpha(const float alpha)
{
	this->alpha = alpha;
}

void IGWindow::beforeRender(Game* g)
{
	prevAlpha = ImGui::GetStyle().Alpha;

	ImGui::GetStyle().Alpha = alpha;


}

void IGWindow::afterRender(Game* g)
{
	ImGui::GetStyle().Alpha = prevAlpha;
}

void IGWindow::setSize(sf::Vector2f size)
{
	this->size = size;
}

void IGWindow::setPosition(sf::Vector2f position, bool centered)
{
	this->position = position;
	this->centeredPosition = true;
}
