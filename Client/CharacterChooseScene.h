#ifndef CHARACTER_CHOOSE_SCENE_H
#define CHARACTER_CHOOSE_SCENE_H


#include "Scene.h"

class CharacterChooseScene :
	public Scene {
public:
	virtual ~CharacterChooseScene() = default;
	CharacterChooseScene(SceneType sceneType);

	void beforeChange(Game* g) override;
	void afterChange(Game* g) override;
	void update(Game* g, sf::Time elapsedTime) override;
	void render(Game* g) override;
};

#endif
