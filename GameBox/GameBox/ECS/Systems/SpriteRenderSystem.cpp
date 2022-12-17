#include "SpriteRenderSystem.h"

SpriteRenderSystem::SpriteRenderSystem(Game* pGame) : m_pGame(pGame) {
}

SpriteRenderSystem::~SpriteRenderSystem() {
}

void SpriteRenderSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) {
	es.each<sf::Sprite>([&](entityx::Entity entity, const sf::Sprite& sprite) {
		m_pGame->getWindow()->draw(sprite);
		});

	if (m_renderSelectionRect) {
		sf::RectangleShape rectangle(sf::Vector2f(m_selectionRect.width, m_selectionRect.height));
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setOutlineThickness(2);
		rectangle.setPosition(m_selectionRect.left, m_selectionRect.top);
		m_pGame->getWindow()->draw(rectangle);
	}
}

void SpriteRenderSystem::updateSelectionRectangle(const sf::FloatRect& rect) {
	m_selectionRect = rect;
}

void SpriteRenderSystem::setRenderSelectionRectangle(const bool renderSelectionRect) {
	m_renderSelectionRect = renderSelectionRect;
}
