#pragma once

#include <SFML\Graphics\Sprite.hpp>

class GameObject : public sf::Sprite
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void update(float dt) = 0;
private:

};