#include "AISystem.h"
#include "SFML/Graphics/RenderWindow.hpp"

AISystem::AISystem(sf::RenderWindow* window) : m_window(window)
{
}

AISystem::~AISystem()
{
}

void AISystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	es.each<AIComponent, sf::Sprite>([&](entityx::Entity entity, AIComponent& aicomponent, sf::Sprite& sprite) {

		if (aicomponent.ball.valid()) {
			if (aicomponent.ball.has_component<sf::Sprite>()) {
				entityx::ComponentHandle<sf::Sprite> spriteCompHND;
				es.unpack<sf::Sprite>(aicomponent.ball.id(), spriteCompHND);
				

				if (spriteCompHND.get()->getPosition().y < sprite.getPosition().y + sprite.getGlobalBounds().width * 0.4f)
				{
					sprite.move(0, -dt * 600);
					if (sprite.getPosition().y < 0)
					{
						sprite.setPosition(sprite.getPosition().x, 0.0f);
					}
				}
				else if (spriteCompHND.get()->getPosition().y > sprite.getPosition().y - sprite.getGlobalBounds().width * 0.4f)
				{
					sprite.move(0, dt * 600);
					if (sprite.getPosition().y > m_window->getSize().y - sprite.getGlobalBounds().height)
					{
						sprite.setPosition(sprite.getPosition().x, m_window->getSize().y - sprite.getGlobalBounds().height);
					}
				}
			}
		}

		
		});

}
