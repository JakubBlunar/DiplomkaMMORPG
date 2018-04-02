#include "Scene.h"

Scene::Scene(std::string name)
{
	this->name = name;
	windowManager = new IGManager();
}


Scene::~Scene()
{
	delete windowManager;
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
