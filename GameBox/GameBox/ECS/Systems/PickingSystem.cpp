#include "PickingSystem.h"
#include "SFML/Graphics/Sprite.hpp"
#include "../../events/ClickActionEvent.h"
#include "AISystem.h"
#include "CollisionSystem.h"

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

	// Iterate over all entities with a "Selectablecomponent"
	es.each<SelectableComponent, sf::Sprite>([&](entityx::Entity entity, SelectableComponent& selectedComp, sf::Sprite& sprite) {
		// If the sprite's bounds intersect with the selection rect,
		// select the entity.
		if (sprite.getGlobalBounds().contains(mousePos)) {
			if (!entity.has_component<SelectedComponent>()) {
				entity.assign<SelectedComponent>();
				sprite.setColor(sf::Color::Blue);
			}
		}

	});

	// Place the blueprint object
	es.each<FollowMouseComponent, sf::Sprite>([&](entityx::Entity entity, FollowMouseComponent& selectedComp, sf::Sprite& sprite) {

		entity.remove<FollowMouseComponent>();
		if (!entity.has_component<CollisionComponent>())
		{
			entity.assign<CollisionComponent>();
		}
		sprite.setColor(sf::Color::White);
		
	});
	m_isBlueprintActive = false;
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
	es.each<SelectableComponent, sf::Sprite>([&](entityx::Entity entity, SelectableComponent& selectedComp, sf::Sprite& sprite) {
		// If the sprite's bounds intersect with the selection rect and the entity is not a map entity,
		// select the entity.
		if (rect.intersects(sprite.getGlobalBounds())) {
			if (!entity.has_component<SelectedComponent>()) {
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

	es.each<FollowMouseComponent, sf::Sprite>([&](entityx::Entity entity, FollowMouseComponent& selectedComp, sf::Sprite& sprite) {
		entity.destroy();
		m_isBlueprintActive = false;
	});

	ClickActionEvent data;
	data.mousePos = mousePos;

	events.emit<ClickActionEvent>(data);

}

bool const PickingSystem::getIsBlueprintActive()
{
	return m_isBlueprintActive;
}

void PickingSystem::setIsBlueprintActive(bool flag)
{
	m_isBlueprintActive = flag;
}