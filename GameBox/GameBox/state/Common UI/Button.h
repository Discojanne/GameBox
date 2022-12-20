#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include <string>

class Button : public sf::Drawable {
public:
	Button(sf::String text, const sf::Font& font, const std::function<void()> func);
	~Button();
	void Trigger();
	void SetIsSelected(bool b);
	void setPosition(float x, float y);
	sf::FloatRect GetRectangle();
private:
	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	std::function<void()> m_func;
	bool m_isSelected = false;
	sf::Text m_text;
};