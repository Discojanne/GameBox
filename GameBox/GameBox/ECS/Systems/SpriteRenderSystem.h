#pragma once
#include "entityx/entityx.h"
#include "SFML/Graphics.hpp"
#include "../../Game.h"

struct FollowMouseComponent
{
};

class SpriteRenderSystem : public entityx::System<SpriteRenderSystem> {
public:
	explicit SpriteRenderSystem(Game* pGame);
	~SpriteRenderSystem();
	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

	bool const getIsBlueprintActive();
	void setIsBlueprintActive(bool flag);
private:
	Game* m_pGame;
	bool m_isBlueprintActive = false;
};