#include "GameState.h"
#include "../Game.h"
#include "SFML/Graphics.hpp"
#include <unordered_map>

#include "../ECS/Systems.h"

GameState::GameState(Game* pGame) : State(States::Game, pGame), m_game(pGame) {

	initializeSystems();
	initializeAnimations();
	initializeEntities();

}

GameState::~GameState() {
	
}

void GameState::update(float dt) {

	systems.update<AnimationSystem>(dt);
	systems.update<TargetingSystem>(dt);
	systems.update<BallSystem>(dt);
	systems.update<AISystem>(dt);

	systems.update<SpriteRenderSystem>(dt);
	systems.update<TextSystem>(dt);
	
}

void GameState::processInput(float dt) {

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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		//Create an animated fire entity that follows the player
		entityx::Entity ent2 = entities.create();
		auto spriteComp = ent2.assign<sf::Sprite>().get();
		spriteComp->setPosition(500, 500);
		spriteComp->setTexture(TextureHandler::getInstance().getTexture("../Resources/player.png"));
		//spriteComp->setOrigin(spriteComp->getLocalBounds().width / 2.0f, spriteComp->getLocalBounds().height / 2.0f);
		spriteComp->setOrigin(0, 0);

		ent2.assign<BallComponent>();
	}

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
	systems.add<AISystem>(m_game->getWindow());
	systems.add<TextSystem>(m_game->getWindow(), events);

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
		auto padcomp = m_playerEntity.assign<PaddelComponent>();
		padcomp->playerID = 0;

		{
			//Create fire entity to render the ball over the fire
			//Continuing the fire init further down
			entityx::Entity ent4 = entities.create();
			auto spriteComp3 = ent4.assign<sf::Sprite>().get();

			//Create a ball entity that follows the player
			entityx::Entity ent2 = entities.create();
			auto spriteComp = ent2.assign<sf::Sprite>().get();
			spriteComp->setPosition(500, 500);
			spriteComp->setTexture(TextureHandler::getInstance().getTexture("../Resources/player.png")); 
			//spriteComp->setOrigin(spriteComp->getLocalBounds().width / 2.0f, spriteComp->getLocalBounds().height / 2.0f);
			spriteComp->setOrigin(0,0);

			ent2.assign<BallComponent>();

			//Create 10 entities that follow the ball at different speeds
			for (size_t i = 0; i < 10; i++) {
				entityx::Entity ent3 = entities.create();
				auto spriteComp2 = ent3.assign<sf::Sprite>().get();
				spriteComp2->setPosition(i * 100, i * 100 % 400);
				spriteComp2->setTexture(TextureHandler::getInstance().getTexture("default.png"));

				ent3.assign<PaddelComponent>();

				auto targetComp = ent3.assign<TargetingComponent>().get();
				targetComp->target = ent2;
				targetComp->speed = 35 + i * 18;
			}

			//Create an animated fire entity that does  follow the ball
			spriteComp3->setPosition(1380, 370);
			spriteComp3->setTexture(TextureHandler::getInstance().getTexture("../Resources/fireSprite.png"));
			spriteComp3->setScale(0.5f, 0.5f);
			spriteComp3->setOrigin(18,86);
			ent4.assign<AnimationComponent>();

			auto targetComp = ent4.assign<TargetingComponent>().get();
			targetComp->target = ent2;
			targetComp->speed = 10000.0f;

			//Create Opponent
			entityx::Entity opponentEntity = entities.create();//Store the player entity in m_playerEntity for future use
			auto opponentEntitySprite = opponentEntity.assign<sf::Sprite>().get();
			opponentEntitySprite->setTexture(TextureHandler::getInstance().getTexture("../Resources/paddel.png"));
			opponentEntitySprite->setPosition(m_game->getWindow()->getSize().x - 25 - comp->getGlobalBounds().width, m_game->getWindow()->getSize().y / 2.0f - comp->getGlobalBounds().height / 2.0f);
			opponentEntitySprite->setColor(sf::Color::Red);
			auto padcomp2 = opponentEntity.assign<PaddelComponent>();
			padcomp2->playerID = 1;
			auto aicomp = opponentEntity.assign<AIComponent>();
			aicomp->ball = ent2;
		}
		
		{
			entityx::Entity testtextEntity = entities.create();
			auto textcomp = testtextEntity.assign<TextComponent>().get();
			textcomp->text.setFont(m_game->GetFont());
			textcomp->text.setString("Player 1");
			textcomp->text.setPosition(m_game->getWindow()->getSize().x / 4.0f - textcomp->text.getGlobalBounds().width / 2, 0.0f);

			systems.system<TextSystem>().get()->setPlayerText(testtextEntity, 0);
		}
		{
			entityx::Entity testtextEntity = entities.create();
			auto textcomp = testtextEntity.assign<TextComponent>().get();
			textcomp->text.setFont(m_game->GetFont());
			textcomp->text.setString("Player 2");
			textcomp->text.setPosition(m_game->getWindow()->getSize().x *3 / 4.0f - textcomp->text.getGlobalBounds().width / 2, 0.0f);

			systems.system<TextSystem>().get()->setPlayerText(testtextEntity, 1);
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
