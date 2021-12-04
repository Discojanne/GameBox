#include "GameState.h"
#include "../Game.h"
#include "SFML/Graphics.hpp"
#include <unordered_map>

#include "../ECS/Systems.h"

GameState::GameState(Game* pGame) : State(States::Game, pGame), m_game(pGame) {


	initializeSystems();
	initializeAnimations();
	initializeEntities();

	m_BackgroundSprite.setTexture(TextureHandler::getInstance().getTexture("background2.jpg"));
	m_gameObjectList.push_back(new Cannonball(TextureHandler::getInstance().getTexture("player.png")));

	this->stream << 0;
	this->text.setString(stream.str()); //texten i spelet
	this->text.setFont(m_game->GetFont());
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

	systems.update<AnimationSystem>(dt);
	systems.update<TargetingSystem>(dt);
	systems.update<BallSystem>(dt);

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
		auto playerSprite = m_playerEntity.component<sf::Sprite>().get();
		playerSprite->move(0, -dt * 600);
		if (playerSprite->getPosition().y < 0)
		{
			playerSprite->setPosition(playerSprite->getPosition().x, 0.0f);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		auto playerSprite = m_playerEntity.component<sf::Sprite>().get();
		playerSprite->move(0, dt * 600);
		if (playerSprite->getPosition().y > m_game->getWindow()->getSize().y - playerSprite->getGlobalBounds().height)
		{
			playerSprite->setPosition(playerSprite->getPosition().x, m_game->getWindow()->getSize().y - playerSprite->getGlobalBounds().height);
		}
	}
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_playerEntity.component<sf::Sprite>().get()->move(-dt * 250, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_playerEntity.component<sf::Sprite>().get()->move(dt * 250, 0);
	}*/
}


void GameState::handleWindowEvent(const sf::Event& windowEvent) {
	switch (windowEvent.type) {
	case sf::Event::EventType::KeyPressed:
		if (windowEvent.key.code == sf::Keyboard::Escape) {
			m_game->SetState(States::MainMenu);
		}
	case sf::Event::MouseButtonPressed:


		// Mouse buttons
		switch (windowEvent.key.code)
		{
		case sf::Mouse::Left:

			if (windowEvent.type == 9)
			{
				//std::cout << "mouse pos: " << windowEvent.mouseButton.x << " " << windowEvent.mouseButton.y << std::endl;
				//m_playerEntity.component<sf::Sprite>().get()->setPosition(windowEvent.mouseButton.x, windowEvent.mouseButton.y);
			}

			break;
		default:
			break;
		}

		break;
		// End of mouse buttons

		break;
	default:
		break;
	}
}

void GameState::initializeSystems() {
	//Setup EntityX Systems
	systems.add<TargetingSystem>();
	systems.add<BallSystem>(m_game->getWindow());
	auto animationSystem = systems.add<AnimationSystem>();
	systems.add<SpriteRenderSystem>(m_game);
	systems.configure();
}

void GameState::initializeAnimations() {
	//Register all animated sprite sheets here before use

	AnimationSystem* pAnimationSystem = systems.system<AnimationSystem>().get();
	if (pAnimationSystem) {
		//Setup Fire Animation
		{
			AnimatedSheetDescription animDesc(TextureHandler::getInstance().getTexture("../Resources/fireSprite.png").getSize(), 8, 4);
			animDesc.AddAnimation(32);
			pAnimationSystem->AddAnimatedSheetDescription(&TextureHandler::getInstance().getTexture("../Resources/fireSprite.png"), animDesc);
		}
		//Setup Character Animation
		{
			AnimatedSheetDescription animDesc(TextureHandler::getInstance().getTexture("../Resources/character.png").getSize(), 7, 3);
			animDesc.AddAnimation(7);
			pAnimationSystem->AddAnimatedSheetDescription(&TextureHandler::getInstance().getTexture("../Resources/character.png"), animDesc);
		}
	}
}

void GameState::initializeEntities() {
	//===Create Entities===
	{
		//Create Player
		m_playerEntity = entities.create();//Store the player entity in m_playerEntity for future use
		auto comp = m_playerEntity.assign<sf::Sprite>().get();
		comp->setPosition(25, m_game->getWindow()->getSize().y / 2.0f - comp->getGlobalBounds().height / 2.0f);
		comp->setTexture(TextureHandler::getInstance().getTexture("../Resources/paddel.png"));
		m_playerEntity.assign<PaddelComponent>();

		{
			//Create an animated fire entity that follows the player
			entityx::Entity ent2 = entities.create();
			auto spriteComp = ent2.assign<sf::Sprite>().get();
			spriteComp->setPosition(500, 500);
			spriteComp->setTexture(TextureHandler::getInstance().getTexture("../Resources/player.png")); 
			//spriteComp->setOrigin(spriteComp->getLocalBounds().width / 2.0f, spriteComp->getLocalBounds().height / 2.0f);
			spriteComp->setOrigin(0,0);

			ent2.assign<BallComponent>();

			//Create 10 entities that follow the player at different speeds
			for (size_t i = 0; i < 10; i++) {
				entityx::Entity ent3 = entities.create();
				auto spriteComp2 = ent3.assign<sf::Sprite>().get();
				spriteComp2->setPosition(i * 100, i * 100 % 400);
				spriteComp2->setTexture(TextureHandler::getInstance().getTexture("default.png"));

				ent3.assign<PaddelComponent>();

				auto targetComp = ent3.assign<TargetingComponent>().get();
				targetComp->target = ent2;
				targetComp->speed = 35 + i * 5;
			}
		}
		{
			//Create an animated fire entity that does not follow the player
			entityx::Entity ent2 = entities.create();
			auto spriteComp = ent2.assign<sf::Sprite>().get();
			spriteComp->setPosition(1380, 370);
			spriteComp->setTexture(TextureHandler::getInstance().getTexture("../Resources/fireSprite.png"));
			spriteComp->setScale(2, 2);
			ent2.assign<AnimationComponent>();
		}

		{
			//Create the animated character entity
			entityx::Entity ent2 = entities.create();
			auto spriteComp = ent2.assign<sf::Sprite>().get();
			spriteComp->setPosition(500, 650);
			spriteComp->setTexture(TextureHandler::getInstance().getTexture("../Resources/character.png"));
			auto animationComp = ent2.assign<AnimationComponent>();
			animationComp->m_animationSpeed = 16;
		}
	}
}
