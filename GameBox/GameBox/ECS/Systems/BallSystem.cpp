#include "BallSystem.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "../../events/ScoreEvent.h"

BallSystem::BallSystem(sf::RenderWindow* window) : m_window(window)
{
}

BallSystem::~BallSystem()
{
}

void BallSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	es.each<BallComponent, sf::Sprite>([&](entityx::Entity entity, BallComponent& ballcomponent, sf::Sprite& sprite) {

		sprite.move(ballcomponent.dir * (float)(ballcomponent.speed * dt));
		sf::Vector2f pos = sprite.getPosition();
		float x_offset = sprite.getGlobalBounds().width;
		float y_offset = sprite.getGlobalBounds().height;




		es.each<PaddelComponent, sf::Sprite>([&](entityx::Entity entity, const PaddelComponent& paddelcomponent, sf::Sprite& spritePaddel) {
			if (sprite.getGlobalBounds().intersects(spritePaddel.getGlobalBounds()))
			{
				float dy = (sprite.getPosition().y + sprite.getGlobalBounds().height / 2.0f) - (spritePaddel.getPosition().y + spritePaddel.getGlobalBounds().height / 2.0f);
				//std::cout << spritePaddel.getGlobalBounds().height << std::endl;
				
				ballcomponent.dir.x *= -1;
				ballcomponent.dir.y = dy / spritePaddel.getGlobalBounds().height;
				float length = sqrtf(ballcomponent.dir.x * ballcomponent.dir.x + ballcomponent.dir.y * ballcomponent.dir.y);
				ballcomponent.dir.x = ballcomponent.dir.x / length;
				ballcomponent.dir.y = ballcomponent.dir.y / length;

				if (abs(ballcomponent.dir.x) < 0.5f)
				{
					ballcomponent.dir.x = 0.5f;

					length = sqrtf(ballcomponent.dir.x * ballcomponent.dir.x + ballcomponent.dir.y * ballcomponent.dir.y);
					ballcomponent.dir.x = ballcomponent.dir.x / length;
					ballcomponent.dir.y = ballcomponent.dir.y / length;
				}
			}
			});


		
		// Left side
		if (pos.x < 0.0f && ballcomponent.dir.x < 0.0f)
		{
			//ballcomponent.dir.x *= -1;
			//sprite.setPosition(0, pos.y);
			
			es.each<PaddelComponent, sf::Sprite>([&](entityx::Entity entity, PaddelComponent& paddelcomponent, sf::Sprite& spritePaddel) {
				if (paddelcomponent.playerID == 1)
				{
					ScoreEvent data;
					data.playerID = 1;
					data.score = ++paddelcomponent.score;
					events.emit<ScoreEvent>(data);
				}
				});
			ballcomponent.dir = sf::Vector2f(1.0f, 0.0f);
			sprite.setPosition(m_window->getSize().x / 2.0f, m_window->getSize().y / 2.0f);

		}
		// Right side
		else if (pos.x > m_window->getSize().x - x_offset && ballcomponent.dir.x > 0.0f)
		{
			//ballcomponent.dir.x *= -1;
			//sprite.setPosition(m_window->getSize().x - x_offset, pos.y);
			es.each<PaddelComponent, sf::Sprite>([&](entityx::Entity entity, PaddelComponent& paddelcomponent, sf::Sprite& spritePaddel) {
				if (paddelcomponent.playerID == 0)
				{
					ScoreEvent data;
					data.playerID = 0;
					data.score = ++paddelcomponent.score;
					events.emit<ScoreEvent>(data);
				}
				});
			ballcomponent.dir = sf::Vector2f(-1.0f, 0.0f);
			sprite.setPosition(m_window->getSize().x / 2.0f, m_window->getSize().y / 2.0f);
		}
		


		// Vertical
		if (pos.y < 0 && ballcomponent.dir.y < 0.0f)
		{
			ballcomponent.dir.y *= -1;
			sprite.setPosition(pos.x, 0.0f);
		}
		else if (pos.y > m_window->getSize().y - y_offset && ballcomponent.dir.y > 0.0f)
		{
			ballcomponent.dir.y *= -1;
			sprite.setPosition(pos.x, m_window->getSize().y - y_offset);
		}

		});

	

}
