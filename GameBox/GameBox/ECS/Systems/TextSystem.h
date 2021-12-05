#pragma once
#include "entityx/entityx.h"
#include "SFML/Graphics/Text.hpp"

namespace sf {
	class RenderWindow;
}


struct TextComponent
{
	sf::Text text;
};

class TextSystem : public entityx::System<TextSystem>
{
public:
	TextSystem(sf::RenderWindow* window);
	~TextSystem();

	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:
	sf::RenderWindow* m_window;
};