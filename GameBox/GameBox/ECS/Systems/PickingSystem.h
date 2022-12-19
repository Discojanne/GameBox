#pragma once

#include "entityx/entityx.h"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Rect.hpp"

struct SelectedComponent
{

};

struct SelectableComponent
{};

class PickingSystem : public entityx::System<PickingSystem>
{
public:
	PickingSystem();
	~PickingSystem();

	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

	/*
		Call when clicking on screen to check if any sprite was clicked on.

		Previously selected sprites will have their selection removed unless "shiftclick" is set to true.
	*/
	void clickLeft(entityx::EntityManager& es, entityx::EventManager& events, const sf::Vector2f& mousePos, bool shiftclick);
	/*
		Call when you want to select all sprites withing a given rectangle.

		Previously selected sprites will have their selection removed unless "shiftclick" is set to true.
	*/
	void selectEntitiesInArea(entityx::EntityManager& es, entityx::EventManager& events, const sf::FloatRect& rect, bool shiftclick);

	void clickRight(entityx::EntityManager& es, entityx::EventManager& events, const sf::Vector2f& mousePos);

	bool const getIsBlueprintActive();
	void setIsBlueprintActive(bool flag);

private:

	entityx::Entity target;

	bool m_isBlueprintActive = false;
};
