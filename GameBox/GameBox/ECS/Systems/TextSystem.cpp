#include "TextSystem.h"
#include "SFML/Graphics.hpp"
#include <sstream>

TextSystem::TextSystem(sf::RenderWindow* window, entityx::EventManager& events) : m_window(window)
{
	m_eventmanager = &events;
	m_eventmanager->subscribe<ScoreEvent>(*this);
}

TextSystem::~TextSystem()
{
	m_eventmanager->unsubscribe<ScoreEvent>(*this);
}

void TextSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{

	es.each<TextComponent>([&](entityx::Entity entity, const TextComponent textcomponent) {
		
		m_window->draw(textcomponent.text);
		});

}

void TextSystem::receive(const ScoreEvent& event)
{
	std::stringstream stream;
	stream << "Score player " << event.playerID + 1 << ": " << event.score;
	players[event.playerID].component<TextComponent>().get()->text.setString(stream.str());
}

void TextSystem::setPlayerText(entityx::Entity entity, int playernumber)
{
	players[playernumber] = entity;
}
