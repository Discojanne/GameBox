#pragma once
#include "entityx/entityx.h"
#include "SFML/Graphics/Sprite.hpp"

namespace sf {
	class RenderWindow;
}


struct BallComponent
{
	float speed = 1000.0f;
	sf::Vector2f dir = sf::Vector2f(-1.0f, 0.0f);
};

struct PaddelComponent
{
};

class BallSystem : public entityx::System<BallSystem>
{
public:
	explicit BallSystem(sf::RenderWindow* window);
	~BallSystem();
	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;
private:
	sf::RenderWindow* m_window;
};
