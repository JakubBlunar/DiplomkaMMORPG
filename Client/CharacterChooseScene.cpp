#include "CharacterChooseScene.h"
#include "IGAvailableCharacters.h"

CharacterChooseScene::CharacterChooseScene(SceneType sceneType, Game* g) : Scene(sceneType, g) {
	IGAvailableCharacters* selectCharactersWindow = new IGAvailableCharacters();
	windowManager->addWindow("selectCharacter", selectCharactersWindow);
	windowManager->OpenAll();
}


void CharacterChooseScene::beforeChange() {
	Scene::beforeChange();
}

void CharacterChooseScene::afterChange() {
	Scene::afterChange();
}

void CharacterChooseScene::update(sf::Time elapsedTime) {
	Scene::update(elapsedTime);
}

void CharacterChooseScene::render() {
	game->window->clear(sf::Color::Green);

	Scene::render();
}
