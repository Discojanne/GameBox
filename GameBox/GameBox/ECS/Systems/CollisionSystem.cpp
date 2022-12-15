#include "CollisionSystem.h"

CollisionSystem::CollisionSystem()
{
}

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	es.each<CollisionComponent, sf::Sprite>([&](entityx::Entity entity, CollisionComponent& selectedComp, sf::Sprite& sprite) {
	

		});

}
