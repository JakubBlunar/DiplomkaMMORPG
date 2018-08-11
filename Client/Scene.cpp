#include "Scene.h"

Scene::Scene(SceneType sceneType)
{
	this->type = sceneType;
	windowManager = new IGManager();
}


Scene::~Scene()
{
	delete windowManager;
}

SceneType Scene::getType() const
{
	return type;
}

void Scene::beforeChange(Game * g)
{
	canChange = false;
}

void Scene::afterChange(Game * g)
{
	canChange = true;
}

void Scene::update(Game * g, sf::Time elapsedTime)
{

}

void Scene::render(Game * g)
{
	windowManager->drawAll(g);
}
