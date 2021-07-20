#include "GameState.h"
#include "../Game.h"


GameState::GameState(Game* pGame) : State(States::Game, pGame), m_Cannonball(), m_animatedFireSprite(64, 128) {
	m_BackgroundTexture.loadFromFile("../Resources/background2.jpg");
	m_BackgroundSprite.setTexture(m_BackgroundTexture);


	this->stream << 0;
	this->text.setString(stream.str()); //texten i spelet
	this->text.setFont(m_game->GetFont());

	m_Cannonball.Init();

	m_FireTextureSheet.loadFromFile("../Resources/fireSprite.png");
	m_animatedFireSprite.setTexture(m_FireTextureSheet);
	m_animatedFireSprite.setPosition(1380, 370);
	m_animatedFireSprite.addAnimation(32);
	m_animatedFireSprite.setScale(2, 2);
}

GameState::~GameState() {
}

void GameState::update(float dt) {

	m_Cannonball.Update(dt);
	m_animatedFireSprite.update(dt);

	this->stream.str(""); // Clear
	this->stream
		<< "x: " << this->m_Cannonball.getPos().x << std::endl
		<< "Vx: " << m_Cannonball.getVelocity().x << std::endl
		<< "Vy: " << m_Cannonball.getVelocity().y << std::endl
		<< "Resistans: " << m_Cannonball.calculateDragforce();

	this->text.setString(stream.str());	/* Update text with new stream */

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
	target.draw(m_animatedFireSprite, states);
	target.draw(this->text);
}