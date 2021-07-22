#include "GameState.h"
#include "../Game.h"
#include "SFML/Graphics.hpp"

struct TargetingComponent {
	entityx::Entity target;
	float speed;
};

class TargetingSystem : public entityx::System<TargetingSystem> {
public:
	explicit TargetingSystem(Game* pGame) : m_pGame(pGame) {
	};

	~TargetingSystem() {
	};

	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override {
		es.each<TargetingComponent, sf::Sprite>([&](entityx::Entity entity, const TargetingComponent& target, sf::Sprite& sprite) {

			if (target.target.valid()) {
				if (target.target.has_component<sf::Sprite>()) {
					entityx::ComponentHandle<sf::Sprite> spriteCompHND;
					es.unpack<sf::Sprite>(target.target.id(), spriteCompHND);
					spriteCompHND.get()->getPosition();

					sf::Vector2f dir = spriteCompHND.get()->getPosition() - sprite.getPosition();
					dir /= std::sqrt(dir.x * dir.x + dir.y * dir.y);
					sprite.setPosition(sprite.getPosition() + dir * target.speed * (float)dt);
				}
			}
			});
	}
private:
	Game* m_pGame;
};

class SpriteRenderSystem : public entityx::System<SpriteRenderSystem> {
public:
	explicit SpriteRenderSystem(Game* pGame) : m_pGame(pGame) {
	};

	~SpriteRenderSystem() {
	};

	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override {
		es.each<sf::Sprite>([&](entityx::Entity entity, const sf::Sprite& sprite) {
			m_pGame->getWindow()->draw(sprite);
			});
	}
private:
	Game* m_pGame;
};

GameState::GameState(Game* pGame) : State(States::Game, pGame) {

	//======================================
	//Setup EntityX Systems
	systems.add<TargetingSystem>(pGame);
	systems.add<SpriteRenderSystem>(pGame);
	systems.configure();
	//======================================

	{
		m_playerEntity = entities.create();
		auto comp = m_playerEntity.assign<sf::Sprite>().get();
		comp->setPosition(100, 100);
		comp->setTexture(TextureHandler::getInstance().getTexture("../Resources/default.png"));

		for (size_t i = 0; i < 10; i++) {
			entityx::Entity ent2 = entities.create();
			auto spriteComp = ent2.assign<sf::Sprite>().get();
			spriteComp->setPosition(i * 100, i * 100 % 400);
			spriteComp->setTexture(TextureHandler::getInstance().getTexture("default.png"));

			auto targetComp = ent2.assign<TargetingComponent>().get();
			targetComp->target = m_playerEntity;
			targetComp->speed = 35 + i * 5;
		}
	}

	m_BackgroundSprite.setTexture(TextureHandler::getInstance().getTexture("background2.jpg"));
	m_gameObjectList.push_back(new Cannonball(TextureHandler::getInstance().getTexture("player.png")));

	this->stream << 0;
	this->text.setString(stream.str()); //texten i spelet
	this->text.setFont(m_game->GetFont());

	//Setup the Animated Fire
	AnimatedGameObject* m_animatedFireSprite = new AnimatedGameObject(8, 4);
	m_animatedFireSprite->setTexture(TextureHandler::getInstance().getTexture("../Resources/fireSprite.png"));
	m_animatedFireSprite->setPosition(1380, 370);
	m_animatedFireSprite->addAnimation(32);
	m_animatedFireSprite->setScale(2, 2);
	m_gameObjectList.push_back(m_animatedFireSprite);

	//Setup the Animated Character
	AnimatedGameObject* m_animatedCharacterSprite = new AnimatedGameObject(7, 3);
	m_animatedCharacterSprite->setTexture(TextureHandler::getInstance().getTexture("../Resources/character.png"));
	m_animatedCharacterSprite->setPosition(500, 650);
	m_animatedCharacterSprite->addAnimation(7);
	m_animatedCharacterSprite->setAnimationSpeed(16);
	m_animatedCharacterSprite->setAnimation(0);
	m_gameObjectList.push_back(m_animatedCharacterSprite);
}

GameState::~GameState() {
	for (auto& i : m_gameObjectList) {
		delete i;
	}
}

void GameState::update(float dt) {

	for (auto& i : m_gameObjectList) {
		i->update(dt);
	}

	systems.update<TargetingSystem>(dt);

	sf::RenderWindow* window = m_game->getWindow();
	window->draw(m_BackgroundSprite);
	systems.update<SpriteRenderSystem>(dt);
	for (auto& i : m_gameObjectList) {
		window->draw(*i);
	}

	window->draw(this->text);

	this->stream.str("test"); // Clear
	this->text.setString(stream.str());	/* Update text with new stream */
}

void GameState::processInput(float dt) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (!static_cast<Cannonball*>(m_gameObjectList[0])->getIsAirbourne())
			static_cast<Cannonball*>(m_gameObjectList[0])->shoot();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		m_playerEntity.component<sf::Sprite>().get()->move(0, -dt * 100);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		m_playerEntity.component<sf::Sprite>().get()->move(0, dt * 100);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_playerEntity.component<sf::Sprite>().get()->move(-dt * 100, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_playerEntity.component<sf::Sprite>().get()->move(dt * 100, 0);
	}
}


void GameState::handleWindowEvent(const sf::Event& windowEvent) {
	switch (windowEvent.type) {
	case sf::Event::EventType::KeyPressed:
		if (windowEvent.key.code == sf::Keyboard::Escape) {
			m_game->SetState(States::MainMenu);
		}
		break;
	default:
		break;
	}
}