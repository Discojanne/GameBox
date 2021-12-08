#pragma once
#include "entityx/entityx.h"
#include "SFML/Graphics/Sprite.hpp"

struct CollisionComponent
{

};

class CollisionSystem : public entityx::System<CollisionSystem>
{
public:
	CollisionSystem();
	~CollisionSystem();

	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:

};
