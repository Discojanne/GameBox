#pragma once
#include "entityx/entityx.h"
#include "SFML/Graphics.hpp"
#include "../../Game.h"

class ShapeRenderSystem : public entityx::System<ShapeRenderSystem> {
public:
	explicit ShapeRenderSystem(Game* pGame);
	~ShapeRenderSystem();	

	// renders all the CIRCLE shapes
	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;
private:
	Game* m_pGame;
};