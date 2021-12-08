#pragma once

#include "entityx/entityx.h"
#include "SFML/Window/Event.hpp"

struct SelectedComponent
{

};

class PickingSystem : public entityx::System<PickingSystem>
{
public:
	PickingSystem();
	~PickingSystem();

	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

	// Call when clicking on screen to check if any sprite was clicked on.
	void clickLeft(entityx::EntityManager& es, entityx::EventManager& events, const sf::Vector2f& mousePos, bool shiftclick);

	void clickRight(entityx::EntityManager& es, entityx::EventManager& events, const sf::Vector2f& mousePos);
private:

	entityx::Entity target;

};
