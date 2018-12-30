#include "GamePlayScene.h"
#include "IGGameMenu.h"
#include "Camera.h"
#include "Globals.h"
#include "Box2D/Box2D.h"
#include "ResourceHolder.h"
#include "VisibleObjectsCast.h"
#include <iostream>
#include "Astar.h"
#include "sfLine.h"

GamePlayScene::GamePlayScene(SceneType sceneType, Game* g) : Scene(sceneType, g) {
	drawDebugData = false;
	targetEntity = nullptr;

	windowManager->addWindow("GameMenu", new IGGameMenu());
	targetInfoWindow = new IGEntityInfo("target", sf::Vector2f(300, 50));
	windowManager->addWindow("TargetInfo", targetInfoWindow);

	playerInfoWindow = new IGEntityInfo("playerInfo", sf::Vector2f(50, 50));
	windowManager->addWindow("PlayerInfo", playerInfoWindow);

	actionBarWindow = new IGActionBar();
	windowManager->addWindow("ActionBar", actionBarWindow);

	mFont = ResourceHolder<sf::Font>::instance()->get("Sansation.ttf");

	targetArrow.setRadius(8);
	targetArrow.setRotation(180);
	targetArrow.setPointCount(3);
	targetArrow.setFillColor(sf::Color::Yellow);
}


GamePlayScene::~GamePlayScene() {}

void GamePlayScene::beforeChange() {
	Scene::beforeChange();
}

void GamePlayScene::afterChange() {
	Scene::afterChange();
	Player* p = game->getAccount()->getPlayerEntity();
	playerInfoWindow->setEntity(p);
	actionBarWindow->setPlayer(p);

	windowManager->Open("PlayerInfo");
	windowManager->Open("ActionBar");
}

void GamePlayScene::update(sf::Time elapsedTime) {
	Scene::update(elapsedTime);
	Map* map = game->getMap();
	if (map) {
		map->update(elapsedTime, game);
	}
}

void GamePlayScene::render() {
	game->window->clear(sf::Color(220, 220, 220));

	Map* map = game->getMap();

	int width = map->getWidth();
	int height = map->getHeight();

	sf::Vector2f offset = game->getCamera()->getOffset();
	sf::Vector2f resolution = game->getCamera()->getResolution();

	int fromX = (int)(offset.x / FIELD_SIZE) - 1;
	int toX = (int)(fromX + (resolution.x / FIELD_SIZE) + 2);

	int fromY = (int)(offset.y / FIELD_SIZE) - 1;
	int toY = (int)(fromY + (resolution.y / FIELD_SIZE) + 2);

	if (fromX < 0)
		fromX = 0;

	if (toX > width)
		toX = width;

	if (fromY < 0)
		fromY = 0;

	if (toY > height)
		toY = height;

	for (int i = fromX; i < toX; i++) {
		for (int j = fromY; j < toY; j++) {
			Field* field = map->getField(i, j);
			std::vector<RenderSprite*>* layers = field->getLayers();

			auto layerSize = layers->size();
			for (unsigned int k = 0; k < layerSize; k++) {
				RenderSprite* layer = layers->at(k);
				layer->setPosition(i * FIELD_SIZE + 16, j * FIELD_SIZE + 16);
				game->window->draw(*layer);
			}
		}
	}

	if (drawDebugData) {
		MapGrid* mapGrid = map->getGrid();

		if (mapGrid) {
			sf::CircleShape shape(4);
			shape.setOrigin(2, 2);

			fromX = mapGrid->transformXToGrid(offset.x);
			fromY = mapGrid->transformYToGrid(offset.y);

			toX = (int)ceil(mapGrid->transformXToGrid(offset.x + resolution.x));
			toY = (int)ceil(mapGrid->transformYToGrid(offset.y + resolution.y));

			for (int i = fromX; i < toX; i++) {
				for (int j = fromY; j < toY; j++) {
					MapGridSpot* spot = mapGrid->grid->get(i, j);
					if (spot) {
						if (spot->isWall()) {
							shape.setFillColor(sf::Color::Black);
						}
						else {
							shape.setFillColor(sf::Color::White);
						}
						shape.setPosition((float)spot->positionX, (float)spot->positionY);
						game->window->draw(shape);
					}

				}
			}

			for (std::list<sf::Vector2f>::const_iterator it = path.begin(); it != path.end(); ++it) {
				if (std::next(it) != path.end()) {

					sfLine line(sf::Vector2f(it->x, it->y), sf::Vector2f(next(it)->x, next(it)->y));
					line.color = sf::Color::Red;
					line.thickness = 3;
					game->window->draw(line);
				}
			}

		}
	}

	b2World* w = map->getB2World();

	aabb.lowerBound = b2Vec2(offset.x * PIXTOMET, offset.y * PIXTOMET);
	aabb.upperBound = b2Vec2((offset.x + resolution.x) * PIXTOMET, (offset.y + resolution.y) * PIXTOMET);
	queryCallback.foundBodies.clear();

	ClientSettings::instance()->eventsMutex.lock();
	w->QueryAABB(&queryCallback, aabb);
	queryCallback.sortBodies();

	auto bodySize = queryCallback.foundBodies.size();
	for (unsigned int i = 0; i < bodySize; i++) {
		Entity* entity = (Entity*)queryCallback.foundBodies[i]->GetUserData();

		RenderComponent* renderComponent = (RenderComponent*)entity->getComponent(ComponentType::RENDER);
		PositionComponent* positionComponent = (PositionComponent*)entity->getComponent(ComponentType::POSITION);

		if (renderComponent && positionComponent) {
			AnimatedSprite* sprite = renderComponent->getCurrentAnimation();

			b2Vec2 position = queryCallback.foundBodies[i]->GetPosition();
			sf::Vector2i renderOffset = renderComponent->getOffset();
			sf::Vector2f spritePosition = sf::Vector2f(
				ceilNumber(METTOPIX * position.x + renderOffset.x), ceilNumber(METTOPIX * position.y + renderOffset.y));
			sprite->setPosition(spritePosition);


			if (entity->getType() == EntityType::NPC) {
				Npc* npc = (Npc*)entity;
				if (npc->getState() == NpcState::DEAD) {
					continue;
				}
			}

			game->window->draw(*sprite);
			if (targetEntity && targetEntity == entity) {
				targetArrow.setPosition(spritePosition.x + renderComponent->getSize().x / 2.f + 8, spritePosition.y);
				game->window->draw(targetArrow);
			}
		}
	}
	ClientSettings::instance()->eventsMutex.unlock();

	if (drawDebugData)
		w->DrawDebugData();

	Scene::render();
}

void GamePlayScene::onKeyPress(sf::Keyboard::Key key) {
	if (!game->window || !game->window->hasFocus()) {
		return;
	}

	if (windowManager->AnyWindowFocused()) {
		return;
	}

	Player* p = game->getAccount()->getPlayerEntity();
	std::vector<SpellInfo*>* spells = p->getSpells();

	switch (key) {
		case sf::Keyboard::Key::Escape: {
			if (windowManager->isVisible("GameMenu")) {
				windowManager->close("GameMenu");
			}		
			else
				windowManager->Open("GameMenu");
			break;
		}
		case sf::Keyboard::F1: 
			drawDebugData = !drawDebugData;
			break;
		case sf::Keyboard::Num1:
		case sf::Keyboard::Key::Numpad1:
			if (!spells->empty()) {
				p->castSpell(spells->at(0));
			}
			break;
		case sf::Keyboard::Num2:
		case sf::Keyboard::Key::Numpad2:
			if (spells->size() > 1) {
				p->castSpell(spells->at(1));
			}
			break;
		case sf::Keyboard::Num3:
		case sf::Keyboard::Key::Numpad3:
			if (spells->size() > 2) {
				p->castSpell(spells->at(2));
			}
			break;
		case sf::Keyboard::Num4:
		case sf::Keyboard::Key::Numpad4:
			if (spells->size() > 3) {
				p->castSpell(spells->at(3));
			}
			break;
		case sf::Keyboard::Num5:
		case sf::Keyboard::Key::Numpad5:
			if (spells->size() > 4) {
				p->castSpell(spells->at(4));
			}
			break;
		case sf::Keyboard::Num6:
		case sf::Keyboard::Key::Numpad6:
			if (spells->size() > 5) {
				p->castSpell(spells->at(5));
			}
			break;
		case sf::Keyboard::Num7:
		case sf::Keyboard::Key::Numpad7:
			if (spells->size() > 6) {
				p->castSpell(spells->at(6));
			}
			break;
		case sf::Keyboard::Num8:
		case sf::Keyboard::Key::Numpad8:
			if (spells->size() > 7) {
				p->castSpell(spells->at(7));
			}
			break;
		case sf::Keyboard::Num9:
		case sf::Keyboard::Key::Numpad9:
			if (spells->size() > 8) {
				p->castSpell(spells->at(8));
			}
			break;
		case sf::Keyboard::Num0:
		case sf::Keyboard::Key::Numpad0:
			if (spells->size() > 9) {
				p->castSpell(spells->at(9));
			}
			break;
		default: break;
	}
}

void GamePlayScene::onClick(sf::Mouse::Button event, sf::Vector2f position) {
	sf::Vector2f offset = game->getCamera()->getOffset();
	sf::Vector2f relativePosition(position.x - offset.x, position.y - offset.y);

	if (drawDebugData) {
		if (game->window->hasFocus()) {
			Map* map = game->getMap();
			MapGrid* mapGrid = map->getGrid();

			Player* player = map->getPlayer();

			sf::Vector2f pathFindingFrom = sf::Vector2f(24, 24);
			if (player) {
				sf::Vector2f playerSize = player->getSize();
				pathFindingFrom = player->getPosition();
				pathFindingFrom.x += playerSize.x / 2;
				pathFindingFrom.y += playerSize.y / 2;
			}
			Astar aStar(mapGrid);
			int res = aStar.findPath(pathFindingFrom, position);
			if (res == 1) {
				path = aStar.path;
			}
		}
	}

	if (windowManager->anyWindowContainsPoint(relativePosition)) {
		return;
	}

	if (event == sf::Mouse::Left) {
		Map* map = game->getMap();
		b2World* w = map->getB2World();

		sf::Vector2f resolution = game->getCamera()->getResolution();
		aabb.lowerBound = b2Vec2(offset.x * PIXTOMET, offset.y * PIXTOMET);
		aabb.upperBound = b2Vec2((offset.x + resolution.x) * PIXTOMET, (offset.y + resolution.y) * PIXTOMET);

		queryCallback.foundBodies.clear();

		ClientSettings::instance()->eventsMutex.lock();
		w->QueryAABB(&queryCallback, aabb);
		bool found = false;

		auto bodySize = queryCallback.foundBodies.size();
		for (unsigned int i = 0; i < bodySize; i++) {
			Entity* entity = (Entity*)queryCallback.foundBodies[i]->GetUserData();
			EntityType type = entity->getType();
			if (type == EntityType::PLAYER || type == EntityType::NPC) {
				if (entity->containsPoint(position)) {
					targetInfoWindow->setEntity(entity);
					targetEntity = entity;
					windowManager->Open("TargetInfo");
					found = true;
				}
			}
		}

		if (!found) {
			targetInfoWindow->setEntity(nullptr);
			targetEntity = nullptr;
			windowManager->close("TargetInfo");
		}

		ClientSettings::instance()->eventsMutex.unlock();
	}
}
