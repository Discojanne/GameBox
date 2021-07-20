#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_map>

struct AnimationDescription {
	size_t nFrames;
	size_t animationRowStart;
};

class AnimatedSprite : public sf::Sprite {
public:
	AnimatedSprite(size_t frameWidth, size_t frameHeight);
	~AnimatedSprite();

	void update(float dt);
	size_t addAnimation(size_t nFrames);
	void setAnimation(size_t animationIndex, float time = 0);
	void setTexture(const sf::Texture& texture, bool resetRect = false);
	void setAnimationSpeed(float speed = 24);
private:
	void updateSpriteTextureRect();

	size_t m_textureFrameWidth = 64;
	size_t m_textureFrameHeight = 128;

	float m_animationSpeed = 24;

	float m_currentAnimationTime;
	size_t m_currentAnimationFrame;
	size_t m_currentAnimationIndex;
	AnimationDescription m_currentAnimation;
	size_t m_nextAnimationRow;
	size_t m_nTextureColumns;
	size_t m_nTextureRows;

	std::vector < AnimationDescription> m_animations;
};

