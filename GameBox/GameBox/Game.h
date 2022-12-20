#pragma once

#include "state/State.h"
#include "Networking/Game-Layer/NetworkConnection.h"
#include "entityx/Event.h"

class Game {
public:
	Game();
	~Game();

	void handleWindowEvent(const sf::Event& event);
	void update();
	void SetState(States::ID id);

	sf::RenderWindow* getWindow();
	sf::Font& GetFont();

	NetworkConnection m_networkConnection;
private:
	void ApplyNextState();
	sf::RenderWindow m_window;
	sf::Clock m_gameTime;

	sf::Font m_font;

	State* m_currentState = nullptr;
	State* m_nextState = nullptr;

	entityx::EventManager m_eventManager;
};