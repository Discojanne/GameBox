#pragma once
#include "entityx/entityx.h"
#include "SFML/Graphics.hpp"
#include "../../Game.h"


class SpriteRenderSystem : public entityx::System<SpriteRenderSystem> {
public:
	explicit SpriteRenderSystem(Game* pGame);
	~SpriteRenderSystem();
	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

	/*
		Update the "selection rectangle".
		To enable rendering of the "selection rectangle" pass "true" to setRenderSelectionRectangle().
	*/
	void updateSelectionRectangle(const sf::FloatRect& rect);
	/*
		Toggle rendering of the "selection rectangle".
	*/
	void setRenderSelectionRectangle(const bool renderSelectionRect);

private:
	Game* m_pGame;
	sf::FloatRect m_selectionRect;
	bool m_renderSelectionRect = false;
};