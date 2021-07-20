#include "GameState.h"
#include "../Game.h"

GameState::GameState(Game* pGame) : State(States::Game, pGame), m_Cannonball() {
	m_BackgroundTexture.loadFromFile("../Resources/background2.jpg");
	m_BackgroundSprite.setTexture(m_BackgroundTexture);

	this->stream << 0;
	this->text.setString(stream.str()); //texten i spelet
	this->text.setFont(m_game->GetFont());

	m_Cannonball.Init();
}

GameState::~GameState() {
}

void GameState::update(float dt) {
	m_Cannonball.Update(dt);
}

void GameState::processInput(float dt) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (!m_Cannonball.getIsAirbourne())
			m_Cannonball.shoot();
	}

}

void GameState::handleWindowEvent(const sf::Event& windowEvent) {
	switch (windowEvent.type) {
	case sf::Event::EventType::KeyPressed:
		if (windowEvent.key.code == sf::Keyboard::Escape) {
			m_game->SetState(States::MainMenu);
		}
		break;
	default:
		break;
	}
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// Make sure everything in the game is drawn.
	target.draw(m_BackgroundSprite, states);
	target.draw(m_Cannonball, states);
	target.draw(this->text);
}