#pragma once

#include <sstream>
#include "../Cannonball.h"
#include <iostream>
#include "State.h"
#include "../AnimatedSprite.h"

class GameState final : public State {
public:
	GameState(Game* pGame);
	~GameState();

	void update(float dt) override;
	void processInput(float dt) override;

	virtual void handleWindowEvent(const sf::Event& windowEvent) override;

private:
	sf::Texture m_FireTextureSheet;
	sf::Texture m_CharacterTextureSheet;

	sf::Texture m_BackgroundTexture;
	sf::Sprite m_BackgroundSprite;
	Cannonball m_Cannonball;
	AnimatedSprite m_animatedFireSprite;
	AnimatedSprite m_animatedCharacterSprite;

	sf::Text text;
	std::ostringstream stream;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};