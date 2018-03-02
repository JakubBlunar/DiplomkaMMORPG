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

void IGWindow::draw(Game* g)
{
	if (!visible)
	{
		return;
	}

	beforeDraw(g);
	onDraw(g);
	afterDraw(g);
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

void IGWindow::beforeDraw(Game* g)
{
	prevAlpha = ImGui::GetStyle().Alpha;

	ImGui::GetStyle().Alpha = alpha;


}

void IGWindow::afterDraw(Game* g)
{
	ImGui::GetStyle().Alpha = prevAlpha;
}
