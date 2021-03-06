﻿#include "SceneManager.h"
#include "LoginScene.h"
#include "Scene.h"
#include "GamePlayScene.h"
#include "CharacterChooseScene.h"
#include <iostream>


SceneManager::SceneManager(Game *g) : sceneToChange(nullptr) {
	LoginScene* logS = new LoginScene(SceneType::LOGIN, g);
	scenes.insert(std::pair<SceneType, Scene*>(SceneType::LOGIN, logS));
	actualScene = logS;

	GamePlayScene* gpS = new GamePlayScene(SceneType::GAMEPLAY, g);
	scenes.insert(std::pair<SceneType, Scene*>(SceneType::GAMEPLAY, gpS));

	CharacterChooseScene* ccs = new CharacterChooseScene(SceneType::CHARACTER_CHOOSE, g);
	scenes.insert(std::pair<SceneType, Scene*>(SceneType::CHARACTER_CHOOSE, ccs));
}


SceneManager::~SceneManager() {
}

SceneType SceneManager::getTypeOfActualScene() const {
	return actualScene->getType();
}

Scene* SceneManager::getActualScene() const {
	return actualScene;
}

void SceneManager::changeScene(SceneType sceneType) {
	const auto exists = scenes.find(sceneType);
	if (exists != scenes.end()) {
		if (actualScene->canChange) {
			switchScene(exists->second);
		}
		else {
			sceneToChange = exists->second;
		}
	}
}

void SceneManager::render() const {
	actualScene->render();
}

void SceneManager::update(sf::Time elapsedTime) {
	actualScene->update(elapsedTime);
	if (sceneToChange && actualScene->canChange) {
		switchScene(sceneToChange);
	}
}

void SceneManager::onKeyPress(sf::Keyboard::Key key) const {
	actualScene->onKeyPress(key);
}

void SceneManager::onKeyRelease(sf::Keyboard::Key key) const {
	actualScene->onKeyRelease(key);
}

void SceneManager::onClick(sf::Mouse::Button button, sf::Vector2f position) const
{
	actualScene->onClick(button, position);
}

void SceneManager::switchScene(Scene* scene) {
	scene->beforeChange();
	sceneToChange = nullptr;
	actualScene = scene;
	scene->afterChange();
}
