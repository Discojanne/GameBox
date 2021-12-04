#pragma once
#include "entityx/entityx.h"
#include "SFML/Graphics/Sprite.hpp"

namespace sf {
	class RenderWindow;
}

struct AIComponent
{
	// Ball entity
	entityx::Entity ball;
};

class AISystem : public entityx::System<AISystem>
{
public:
	explicit AISystem(sf::RenderWindow* window);
	~AISystem();
	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;
private:
	sf::RenderWindow* m_window;
};