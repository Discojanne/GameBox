#include "LobbyState.h"
#include "../../Game.h"

LobbyState::LobbyState(Game* pGame) : State(States::Lobby, pGame) {

}

LobbyState::~LobbyState() {
}

void LobbyState::handleWindowEvent(const sf::Event& windowEvent) {

}

void LobbyState::processInput(float dt) {

}

void LobbyState::update(float dt) {
	m_game->m_networkConnection.m_players;


	float posX = 50;
	float posY = 150;

	for (auto& player : m_game->m_networkConnection.m_players) {
		sf::Text text;
		std::string playerDisplayName = player.playerName + " (" + std::to_string(player.playerID) + ")";
		if (player.playerID == 0) {
			playerDisplayName += " Host";
		}

		text.setString(playerDisplayName);
		text.setPosition(posX, posY);
		text.setFillColor(sf::Color::Green);
		text.setFont(m_game->GetFont());
		m_game->getWindow()->draw(text);

		posY += 50;
	}
}
