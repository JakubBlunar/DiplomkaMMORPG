#include "IGLoginCredentials.h"
#include "Game.h"


IGLoginCredentials::IGLoginCredentials()
{
	memset(buffLogin, 0, sizeof(buffLogin));
	memset(buffPassword, 0, sizeof(buffPassword));
	setSize(sf::Vector2f(450, 250));
}


IGLoginCredentials::~IGLoginCredentials()
{

}

void IGLoginCredentials::render(Game* game)
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(100, 100, 0, 0));

	ImGui::SetNextWindowSize(size, ImGuiCond_Always);
	ImGui::SetNextWindowPos(position);
	ImGui::SetNextWindowPosCenter(centeredPosition);

	if (!ImGui::Begin("Login", &visible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
	{
		focused = false;
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(-1);

	ImGui::SetWindowFontScale(3);
	ImGui::Text("Log into your account");


	ImGui::SetWindowFontScale(2);
	ImGui::TextColored(ImVec4(0, 0, 0, 1), "Account Name:");
	ImGui::InputText("##Account name", buffLogin, sizeof(buffLogin));

	ImGui::TextColored(ImVec4(0, 0, 0, 1), "Password:");
	ImGui::InputText("##Password", buffPassword, sizeof(buffPassword), ImGuiInputTextFlags_Password);

	if (ImGui::Button("Login")) {
		std::string name = std::string(buffLogin);
		std::string password = std::string(buffPassword);
		memset(buffPassword, 0, sizeof(buffPassword));
		game->print(name + " " + password);
	}

	ImGui::PopItemWidth();

	focused = ImGui::IsWindowFocused();
	ImGui::SetWindowFontScale(1);

	ImGui::End(); // end window

	ImGui::PopStyleColor();
}

void IGLoginCredentials::beforeRender(Game * game)
{
	IGWindow::beforeRender(game);

	float left = game->window.getView().getViewport().left + game->window.getView().getViewport().width / 2.0f;
	float top = game->window.getView().getViewport().top + game->window.getView().getViewport().height / 2.0f;
	setPosition(sf::Vector2f(left, top), true);

}

