#pragma once

#include <sstream>
#include "../Cannonball.h"
#include <iostream>
#include "State.h"


class GameState final : public sf::Drawable, public State {
public:
	GameState();
	~GameState();

	void update(float dt) override;

	void processInput(float dt) override;

private:
	sf::Texture m_BackgroundTexture;
	sf::Sprite m_BackgroundSprite;
	Cannonball m_Cannonball;

	sf::Text text;
	std::ostringstream stream;
	sf::Font font;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};