#include "SceneManager.h"
#include "LoginScene.h"
#include "Scene.h"
#include "GamePlayScene.h"

SceneManager::SceneManager()
{
	LoginScene* logS = new LoginScene(SceneType::LOGIN);
	scenes.insert(std::pair<SceneType, Scene*>(SceneType::LOGIN, logS));
	actualScene = logS;

	GamePlayScene* gpS = new GamePlayScene(SceneType::GAMEPLAY);
	scenes.insert(std::pair<SceneType, Scene*>(SceneType::GAMEPLAY, gpS));
	actualScene = gpS;
}


SceneManager::~SceneManager()
{
}

SceneType SceneManager::getTypeOfActualScene() const
{
	return actualScene->getType();
}

void SceneManager::changeScene(SceneType sceneType)
{
	const auto exists = scenes.find(sceneType);
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
