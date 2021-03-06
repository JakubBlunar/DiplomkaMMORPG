﻿#include "IGPopup.h"
#include "Game.h"
#include "SceneManager.h"

IGPopup::IGPopup(std::string title, std::string text, sf::Vector2f size, std::string buttonText) {
	this->title = title;
	this->text = text;
	this->buttonText = buttonText;
	onButtonClick = []() {
	};
	setAlpha(1);
	setSize(size);
	open();
}

IGPopup::IGPopup(std::string title, std::string text, sf::Vector2f size, std::string buttonText,
	std::function<void()> onButtonClick = []() {
}) {
	this->title = title;
	this->text = text;
	this->buttonText = buttonText;
	this->onButtonClick = onButtonClick;
	setAlpha(1);
	setSize(size);
	open();
}

IGPopup::~IGPopup() {

}

void IGPopup::render(Game* g, IGManager* manager) {
	ImGui::OpenPopup("popup");

	ImGui::SetNextWindowSize(size, ImGuiCond_Always);
	ImGui::SetNextWindowPos(position);
	ImGui::SetNextWindowFocus();

	if (ImGui::BeginPopupModal("popup", &visible,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {
		ImGui::PushItemWidth(-1);

		ImGui::SetWindowFontScale(0.2f);
		if (!title.empty()) {
			ImGui::Text(title.c_str());
			ImGui::NewLine();
		}

		ImGui::TextWrapped(text.c_str());

		if (!buttonText.empty()) {
			ImGui::NewLine();
			if (ImGui::Button(buttonText.c_str(), sf::Vector2f(ImGui::GetWindowWidth() * 0.97f, 40))) {
				close();
				onButtonClick();
			}
		}


		ImGui::PopItemWidth();

		focused = ImGui::IsWindowFocused();
		ImGui::SetWindowFontScale(1);

		ImGui::EndPopup();
	}

}

void IGPopup::beforeRender(Game* g) {
	IGWindow::beforeRender(g);

	sf::Vector2u windowSize = g->window->getSize();

	float left = windowSize.x / 2 - size.x / 2;
	float top = windowSize.y / 2 - size.y / 2;
	setPosition(sf::Vector2f(left, top));
}
