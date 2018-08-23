#include "CharacterChooseScene.h"
#include "IGAvailableCharacters.h"

CharacterChooseScene::CharacterChooseScene(SceneType sceneType) : Scene(sceneType) {
	IGAvailableCharacters* selectCharactersWindow = new IGAvailableCharacters();
	windowManager->addWindow("selectCharacter", selectCharactersWindow);
	windowManager->OpenAll();
}


void CharacterChooseScene::beforeChange(Game* g) {
	Scene::beforeChange(g);
}

void CharacterChooseScene::afterChange(Game* g) {
	Scene::afterChange(g);
}

void CharacterChooseScene::update(Game* g, sf::Time elapsedTime) {
	Scene::update(g, elapsedTime);
}

void CharacterChooseScene::render(Game* g) {
	g->window.clear(sf::Color::Green);

	Scene::render(g);
}
