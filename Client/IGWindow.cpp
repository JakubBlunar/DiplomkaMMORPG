#include "IGWindow.h"
#include <imgui.h>
#include <iomanip>
#include <sstream>
#include <SFML/Graphics/Rect.hpp>


IGWindow::IGWindow() : prevAlpha(0) {
	this->visible = false;
	this->alpha = ImGui::GetStyle().Alpha;
}

IGWindow::IGWindow(const bool visible = false, const float alpha = ImGui::GetStyle().Alpha) : prevAlpha(0) {
	this->visible = visible;
	this->alpha = alpha;
}


IGWindow::~IGWindow() {
}


void IGWindow::draw(Game* g, IGManager* manager) {
	if (!visible) {
		return;
	}

	beforeRender(g);
	render(g, manager);
	afterRender(g);
}

void IGWindow::open() {
	visible = true;
}

void IGWindow::close() {
	visible = false;
}

bool IGWindow::isOpened() const {
	return visible;
}

bool IGWindow::isFocused() const {
	if (!visible)
		return false;

	return focused;
}

float IGWindow::getAlpha() const {
	return alpha;
}

void IGWindow::setAlpha(const float alpha) {
	this->alpha = alpha;
}

void IGWindow::beforeRender(Game* g) {
	prevAlpha = ImGui::GetStyle().Alpha;

	ImGui::GetStyle().Alpha = alpha;


}

void IGWindow::afterRender(Game* g) {
	ImGui::GetStyle().Alpha = prevAlpha;
}

void IGWindow::setSize(sf::Vector2f size) {
	this->size = size;
}

void IGWindow::setPosition(sf::Vector2f position) {
	this->position = position;
}

std::string IGWindow::convertFloatToString(float number, int precision) const {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(precision) << number;
	return stream.str();
}

bool IGWindow::containsPosition(sf::Vector2f point) const {
	sf::Rect<float> window;
	window.top = position.y;
	window.left = position.x;
	window.width = size.x;
	window.height = size.y;

	return window.contains(point);
}
