#ifndef CHARACTER_CHOOSE_SCENE_H
#define CHARACTER_CHOOSE_SCENE_H


#include "Scene.h"

class CharacterChooseScene :
	public Scene, public Subscriber {
public:
	virtual ~CharacterChooseScene() = default;
	CharacterChooseScene(SceneType sceneType, Game* g);

	void beforeChange() override;
	void afterChange() override;
	void update(sf::Time elapsedTime) override;
	void render() override;

	void handleEvent(GameEvent* event) override;

	RenderSprite background;
};

#endif
