#include "PickingSystem.h"
#include "SFML/Graphics/Sprite.hpp"
#include "../../events/ClickActionEvent.h"

PickingSystem::PickingSystem()
{
}

PickingSystem::~PickingSystem()
{
}

void PickingSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	
}

void PickingSystem::clickLeft(entityx::EntityManager& es, entityx::EventManager& events, const sf::Vector2f& mousePos, bool shiftclick)
{
	bool notSpriteHit = true;

	// Loop each sprite to find the target

	es.each<sf::Sprite>([&](entityx::Entity entity, sf::Sprite& sprite) {
		
		if (sprite.getGlobalBounds().contains(mousePos))
		{
			notSpriteHit = false;

			//auto playerSprite = entity.component<sf::Sprite>().get();
			if (!entity.has_component<SelectedComponent>())
			{
				// shift allwos you to select multiple units
				if (!shiftclick)
				{
					es.each<SelectedComponent, sf::Sprite>([&](entityx::Entity entity, SelectedComponent& selectedComp, sf::Sprite& sprite) {
						entity.remove<SelectedComponent>();
						sprite.setColor(sf::Color::White);
						});
				}
				entity.assign<SelectedComponent>();
				sprite.setColor(sf::Color::Blue);
			}
			//playerSprite->setColor(sf::Color::Blue);
			//sprite.setColor(sf::Color::Blue);
		}
		
		});

	if (notSpriteHit && !shiftclick)
	{
		es.each<SelectedComponent, sf::Sprite>([&](entityx::Entity entity, SelectedComponent& selectedComp, sf::Sprite& sprite) {
			entity.remove<SelectedComponent>();
			sprite.setColor(sf::Color::White);
			});
	}
}

void PickingSystem::clickRight(entityx::EntityManager& es, entityx::EventManager& events, const sf::Vector2f& mousePos)
{
	// Loop each sprite to find the target

	//es.each<sf::Sprite>([&](entityx::Entity entity, sf::Sprite& sprite) {

	//	//std::cout << "mouse posinside: " << mouseEvent.mouseButton.x << " " << mouseEvent.mouseButton.y << std::endl;


	//	if (sprite.getGlobalBounds().contains(mousePos))
	//	{
	//		auto playerSprite = entity.component<sf::Sprite>().get();
	//		playerSprite->setColor(sf::Color::Blue);
	//		//sprite.setColor(sf::Color::Blue);
	//	}

	//	});
	ClickActionEvent data;
	data.mousePos = mousePos;

	events.emit<ClickActionEvent>(data);

}
