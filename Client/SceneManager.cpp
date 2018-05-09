#include "SceneManager.h"
#include "LoginScene.h"
#include "Scene.h"
#include "GamePlayScene.h"

SceneManager::SceneManager()
{
	LoginScene* logS = new LoginScene("login");
	scenes.insert(std::pair<std::string, Scene*>(logS->name, logS));
	actualScene = logS;

	GamePlayScene* gpS = new GamePlayScene("gamePlay");
	scenes.insert(std::pair<std::string, Scene*>(gpS->name, gpS));
	actualScene = gpS;
}


SceneManager::~SceneManager()
{
}

void SceneManager::changeScene(std::string name)
{
	const auto exists = scenes.find(name);
	if (exists != scenes.end())
	{	
		actualScene = exists->second;
	}
}

void SceneManager::render(Game * g) const
{
	actualScene->render(g);
}

void SceneManager::update(Game * g, sf::Time elapsedTime) const
{
	actualScene->update(g, elapsedTime);
}
