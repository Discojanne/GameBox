#pragma once

#include "state/GameState.h"
	
class Game
{
public:
	Game();
	~Game();

	void update();
	void render();

	sf::RenderWindow* getWindow();

private:

	sf::RenderWindow m_window;
	sf::Clock m_gameTime;
	GameState m_gameState;
};