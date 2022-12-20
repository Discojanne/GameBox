#include "Button.h"

Button::Button(sf::String text, const sf::Font& font, const std::function<void()> func) : m_func(func) {
	m_text.setFont(font);
	m_text.setString(text);
}

Button::~Button() {
}

void Button::Trigger() {
	m_func();
}

void Button::SetIsSelected(bool b) {
	m_isSelected = b;
	if (m_isSelected) {
		m_text.setStyle(sf::Text::Bold | sf::Text::Italic);
	} else {
		m_text.setStyle(0);
	}
}

void Button::setPosition(float x, float y) {
	m_text.setPosition(x, y);
}

sf::FloatRect Button::GetRectangle() {
	return m_text.getGlobalBounds();
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (m_isSelected) {

	} else {

	}
	target.draw(m_text, states);
}
