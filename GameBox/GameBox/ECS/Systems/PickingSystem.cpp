#include "PickingSystem.h"
#include "SFML/Graphics/Sprite.hpp"
#include "../../events/ClickActionEvent.h"
#include "AISystem.h"

PickingSystem::PickingSystem() {
}

PickingSystem::~PickingSystem() {
}

void PickingSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) {

}

void PickingSystem::clickLeft(entityx::EntityManager& es, entityx::EventManager& events, const sf::Vector2f& mousePos, bool shiftclick) {
	// If shiftclick is false, deselect all previously selected entities.
	if (!shiftclick) {
		es.each<SelectedComponent, sf::Sprite>([&](entityx::Entity entity, SelectedComponent& selectedComp, sf::Sprite& sprite) {
			entity.remove<SelectedComponent>();
			sprite.setColor(sf::Color::White);
			});
	}

	// Iterate over all entities with a Sprite component.
	es.each<sf::Sprite>([&](entityx::Entity entity, sf::Sprite& sprite) {
		// If the sprite's bounds intersect with the selection rect and the entity is not a map entity,
		// select the entity.
		if (sprite.getGlobalBounds().contains(mousePos) && !entity.has_component<MapComponent>()) {
			if (!entity.has_component<SelectedComponent>()) {
				entity.assign<SelectedComponent>();
				sprite.setColor(sf::Color::Blue);
			}
		}

		});
}

void PickingSystem::selectEntitiesInArea(entityx::EntityManager& es, entityx::EventManager& events, const sf::FloatRect& rect, bool shiftclick) {
	// If shiftclick is false, deselect all previously selected entities.
	if (!shiftclick) {
		es.each<SelectedComponent, sf::Sprite>([&](entityx::Entity entity, SelectedComponent& selectedComp, sf::Sprite& sprite) {
			entity.remove<SelectedComponent>();
			sprite.setColor(sf::Color::White);
			});
	}

	// Iterate over all entities with a Sprite component.
	es.each<sf::Sprite>([&](entityx::Entity entity, sf::Sprite& sprite) {
		// If the sprite's bounds intersect with the selection rect and the entity is not a map entity,
		// select the entity.
		if (rect.intersects(sprite.getGlobalBounds()) && !entity.has_component<MapComponent>()) {
			if (!entity.has_component<SelectedComponent>() && !entity.has_component<MapComponent>()) {
				entity.assign<SelectedComponent>();
				sprite.setColor(sf::Color::Blue);
			}
		}
		});
}

void PickingSystem::clickRight(entityx::EntityManager& es, entityx::EventManager& events, const sf::Vector2f& mousePos) {
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
