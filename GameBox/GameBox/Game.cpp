#include "Game.h"
#include <SFML\Graphics.hpp>

Game::Game() : m_window(sf::VideoMode(1600, 800), "Gamebox setup test"),
m_gameState()
{
}

Game::~Game()
{
}

void Game::update()
{
	float dt = m_gameTime.restart().asSeconds();

	m_gameState.processInput(dt);
	m_gameState.update(dt);
}

void Game::render()
{
	m_window.clear();
	m_window.draw(m_gameState);
	m_window.display();
}

sf::RenderWindow* Game::getWindow()
{
	return &m_window;
}
