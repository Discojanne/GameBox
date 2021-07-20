#include "GameState.h"

GameState::GameState() : State()
{
	m_BackgroundSprite.setTexture(TextureHandler::getInstance().getTexture("background2.jpg"));
	//m_Cannonball.Init(TextureHandler::getInstance().getTexture("player.png"));

	m_gameObjectList.push_back(new Cannonball(TextureHandler::getInstance().getTexture("player.png")));

	this->stream << 0;
	this->text.setString(stream.str()); //texten i spelet
	this->font.loadFromFile("../Resources/Arcon-Regular.otf");
	this->text.setFont(font);

	
}

GameState::~GameState()
{
	for (auto& i : m_gameObjectList)
	{
		delete i;
	}
}

void GameState::update(float dt)
{
	//processInput(dt);

	for (auto& i : m_gameObjectList)
	{
		i->update(dt);
	}

	//m_Cannonball.update(dt);

	this->stream.str("test"); // Clear
	/*this->stream
		<< "x: " << this->m_Cannonball.getPos().x << std::endl 
		<< "Vx: " << m_Cannonball.getVelocity().x << std::endl
		<< "Vy: " << m_Cannonball.getVelocity().y << std::endl
		<< "Resistans: " << m_Cannonball.calculateDragforce();*/
	
	this->text.setString(stream.str());	/* Update text with new stream */

}

void GameState::processInput(float dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (!static_cast<Cannonball*>(m_gameObjectList[0])->getIsAirbourne())
			static_cast<Cannonball*>(m_gameObjectList[0])->shoot();
	}
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Make sure everything in the game is drawn.
	target.draw(m_BackgroundSprite, states);

	for (auto& i : m_gameObjectList)
	{
		target.draw(*i, states);
	}

	target.draw(this->text);
}