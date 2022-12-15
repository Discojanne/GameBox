#include "ShapeRenderSystem.h"

ShapeRenderSystem::ShapeRenderSystem(Game* pGame) : m_pGame(pGame)
{
}

ShapeRenderSystem::~ShapeRenderSystem()
{
}

void ShapeRenderSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	es.each<sf::CircleShape>([&](entityx::Entity entity, const sf::Shape& sprite) {
		m_pGame->getWindow()->draw(sprite);
		});
}
