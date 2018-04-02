#include "SceneManager.h"
#include "LoginScene.h"
#include "Scene.h"

SceneManager::SceneManager()
{
	LoginScene* logS = new LoginScene("login");
	scenes.insert(std::pair<std::string, Scene*>(logS->name, logS));
	actualScene = logS;
}


SceneManager::~SceneManager()
{
}

void SceneManager::changeScene(std::string name)
{

}

void SceneManager::render(Game * g)
{
	actualScene->render(g);
}

void SceneManager::update(Game * g, sf::Time elapsedTime)
{
	actualScene->update(g, elapsedTime);
}
