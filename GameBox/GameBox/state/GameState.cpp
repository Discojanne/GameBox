#include "GameState.h"

GameState::GameState() : State(), m_Cannonball()
{
	m_BackgroundTexture.loadFromFile("../Resources/background2.jpg");
	m_BackgroundSprite.setTexture(m_BackgroundTexture);

	this->stream << 0;
	this->text.setString(stream.str()); //texten i spelet
	this->font.loadFromFile("../Resources/Arcon-Regular.otf");
	this->text.setFont(font);

	m_Cannonball.Init();
}

GameState::~GameState()
{
}

void GameState::update(float dt)
{
	//processInput(dt);

	m_Cannonball.Update(dt);

	this->stream.str(""); // Clear
	this->stream
		<< "x: " << this->m_Cannonball.getPos().x << std::endl 
		<< "Vx: " << m_Cannonball.getVelocity().x << std::endl
		<< "Vy: " << m_Cannonball.getVelocity().y << std::endl
		<< "Resistans: " << m_Cannonball.calculateDragforce();
	
	this->text.setString(stream.str());	/* Update text with new stream */

}

void GameState::processInput(float dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (!m_Cannonball.getIsAirbourne())
			m_Cannonball.shoot();
	}

}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Make sure everything in the game is drawn.
	target.draw(m_BackgroundSprite, states);
	target.draw(m_Cannonball, states);
	target.draw(this->text);
}