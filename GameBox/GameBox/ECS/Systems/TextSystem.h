#pragma once
#include "entityx/entityx.h"
#include "SFML/Graphics/Text.hpp"
#include "../../events/ScoreEvent.h"

namespace sf {
	class RenderWindow;
}


enum TextID
{
	PLAYERSCORE,
	OPPONENTSCORE
};

struct TextComponent
{
	sf::Text text;
};

class TextSystem : public entityx::System<TextSystem>, public entityx::Receiver<TextSystem>
{
public:
	TextSystem(sf::RenderWindow* window, entityx::EventManager& events);
	~TextSystem();

	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

	void receive(const ScoreEvent& event);
	void setPlayerText(entityx::Entity entity, int playernumber);
private:
	sf::RenderWindow* m_window;
	entityx::EventManager* m_eventmanager;

	entityx::Entity players[2];
};