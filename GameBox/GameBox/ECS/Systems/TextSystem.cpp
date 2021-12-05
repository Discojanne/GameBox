#include "TextSystem.h"
#include "SFML/Graphics.hpp"

TextSystem::TextSystem(sf::RenderWindow* window) : m_window(window)
{
}

TextSystem::~TextSystem()
{
}

void TextSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{

	es.each<TextComponent>([&](entityx::Entity entity, const TextComponent textcomponent) {
		
		m_window->draw(textcomponent.text);
		});
}
