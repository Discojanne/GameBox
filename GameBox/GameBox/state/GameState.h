#pragma once

#include <sstream>
#include "../Cannonball.h"
#include <iostream>
#include "State.h"

//#include "../AnimatedGameObject.h"
#include "../TextureHandler.h"
#include "../gameObjects/gameObject.h"

class GameState final : public State {
public:
	GameState(Game* pGame);
	~GameState();

	void update(float dt) override;
	void processInput(float dt) override;

	virtual void handleWindowEvent(const sf::Event& windowEvent) override;

private:

	void initializeSystems();
	void initializeAnimations();
	void initializeEntities();

	std::vector<GameObject*> m_gameObjectList;
	sf::Sprite m_BackgroundSprite;
	entityx::Entity m_playerEntity;

	sf::Text text;
	std::ostringstream stream;
};