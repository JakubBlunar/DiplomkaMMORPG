#include "Scene.h"
#include "ResourceHolder.h"

Scene::Scene(SceneType sceneType, Game* g) {
	this->type = sceneType;
	this->game = g;
	windowManager = new IGManager();
	canChange = true;

	messageText.setFont(ResourceHolder<sf::Font>::instance()->get("Sansation.ttf"));
	messageText.setStyle(sf::Text::Style::Bold);
}


Scene::~Scene() {
	delete windowManager;
}

SceneType Scene::getType() const {
	return type;
}

IGManager* Scene::getWindowManager() const {
	return windowManager;
}

void Scene::beforeChange() {
	canChange = false;
}

void Scene::afterChange() {
	canChange = true;
}

void Scene::update(sf::Time elapsedTime) {
	sf::Time gameTime = game->getGameTime();

	while (messages.size() < 5 && !waitingMessages.empty()) {
		GameMessage* message = waitingMessages.front();
		waitingMessages.pop();

		message->startTime = game->getGameTime();
		messages.push_back(message);
	}


	for (int i = 0; i < messages.size(); i++) {
		GameMessage* message = messages.at(i);
		if (gameTime > message->startTime + message->displayTime) {
			messages.erase(std::find(messages.begin(), messages.end(), message));
			delete message;
		}
	}
}

void Scene::render() {
	windowManager->drawAll(game);

	sf::Vector2f offset = game->getCamera()->getOffset();
	sf::Vector2f resolution = game->getCamera()->getResolution();
	float startY = offset.y + resolution.y / 5;
	float baseX = offset.x + resolution.x / 2;

	int count = messages.size();
	for (int i = 0; i < count; i++) {
		GameMessage* message = messages.at(i);

		messageText.setString(message->message);
		messageText.setFillColor(message->color);
		messageText.setCharacterSize(message->size);

		sf::FloatRect bounds = messageText.getGlobalBounds();
		startY += bounds.height + 3;

		messageText.setPosition(baseX - bounds.width / 2, startY);
		game->window->draw(messageText);
	}
}

void Scene::addGameMessage(GameMessage* message) {
	waitingMessages.push(message);
}

void Scene::onClick(sf::Mouse::Button event, sf::Vector2f position) { }

void Scene::onKeyPress(sf::Keyboard::Key key) {}

void Scene::onKeyRelease(sf::Keyboard::Key key) {}
