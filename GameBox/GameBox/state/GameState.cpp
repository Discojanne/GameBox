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

	systems.update<ShapeRenderSystem>(dt);
	systems.update<SpriteRenderSystem>(dt);
	systems.update<TextSystem>(dt);
	
}

void GameState::processInput(float dt) {

	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
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
	}*/


}


void GameState::handleWindowEvent(const sf::Event& windowEvent) {
	switch (windowEvent.type) {
	case sf::Event::EventType::KeyPressed:
		if (windowEvent.key.code == sf::Keyboard::Escape) {
			m_game->SetState(States::MainMenu);
		}
		if (windowEvent.key.code == sf::Keyboard::A) {
			auto translated_pos = m_game->getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*m_game->getWindow()));
			systems.system<AISystem>().get()->tempClickTest(translated_pos);
		}
	case sf::Event::MouseButtonPressed:


		// Mouse buttons
		switch (windowEvent.key.code)
		{
			// Select 
		case sf::Mouse::Left:

			if (windowEvent.type == 9)
			{
				//std::cout << "mouse pos: " << windowEvent.mouseButton.x << " " << windowEvent.mouseButton.y << std::endl;
				std::cout << "Leftclick" << std::endl;
				//m_playerEntity.component<sf::Sprite>().get()->setPosition(windowEvent.mouseButton.x, windowEvent.mouseButton.y);
				auto translated_pos = m_game->getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*m_game->getWindow()));
				//std::cout << "mouse pos2: " << translated_pos.x << " " << translated_pos.y << std::endl;
				systems.system<PickingSystem>().get()->clickLeft(entities, events, translated_pos, sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
				
				
				//std::cout << sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) << std::endl;
				
				//m_playerEntity.component<AIComponent>().get()->state = AI::State::WALKING;
				//m_playerEntity.component<AIComponent>().get()->targetPos = translated_pos;

			}

			break;
			// Action
		case sf::Mouse::Right:

			if (windowEvent.type == 9)
			{
				std::cout << "Rightcluck" << std::endl;

				auto translated_pos = m_game->getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*m_game->getWindow()));
				systems.system<PickingSystem>().get()->clickRight(entities, events, translated_pos);



				//m_playerEntity.component<AIComponent>().get()->state = AI::State::WALKING;
				//m_playerEntity.component<AIComponent>().get()->targetPos = translated_pos;

			}

			break;
		default:
			break;
		}

		break;
		// End of mouse buttons

	default:
		break;
	}
}

void GameState::initializeSystems() {
	//Setup EntityX Systems
	systems.add<TargetingSystem>();
	systems.add<BallSystem>(m_game->getWindow());
	systems.add<AISystem>(m_game->getWindow(), events, entities);
	systems.add<TextSystem>(m_game->getWindow(), events);
	systems.add<PickingSystem>();
	systems.add<CollisionSystem>();

	auto animationSystem = systems.add<AnimationSystem>();
	systems.add<ShapeRenderSystem>(m_game);
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

		for (unsigned int i = 0; i < 10; i++)
		{
			auto ent = entities.create(); //Store the player entity in m_playerEntity for future use
			auto spriteComp = ent.assign<sf::Sprite>().get();
			spriteComp->setPosition(25 + 69 * i, m_game->getWindow()->getSize().y / 2.0f - spriteComp->getGlobalBounds().height / 2.0f);
			spriteComp->setTexture(TextureHandler::getInstance().getTexture("../Resources/cubeman.png"));
			spriteComp->setScale(3, 3);
			ent.assign<AIComponent>();
			ent.assign<CollisionComponent>();
		}

		{
			auto ent = entities.create(); //Store the player entity in m_playerEntity for future use
			auto spriteComp = ent.assign<sf::CircleShape>(20.0f).get();
			spriteComp->setPosition(25 + 7, m_game->getWindow()->getSize().y / 4.0f - spriteComp->getGlobalBounds().height / 2.0f);
			spriteComp->setFillColor(sf::Color::Red);
			
		}

		//Create some entities
		

		{
			//Create fire entity to render the ball over the fire
			//Continuing the fire init further down
			//entityx::Entity ent4 = entities.create();
			//auto spriteComp3 = ent4.assign<sf::Sprite>().get();

			////Create a ball entity that follows the player
			//entityx::Entity ent2 = entities.create();
			//auto spriteComp = ent2.assign<sf::Sprite>().get();
			//spriteComp->setPosition(500, 500);
			//spriteComp->setTexture(TextureHandler::getInstance().getTexture("../Resources/player.png")); 
			////spriteComp->setOrigin(spriteComp->getLocalBounds().width / 2.0f, spriteComp->getLocalBounds().height / 2.0f);
			//spriteComp->setOrigin(0,0);

			//ent2.assign<BallComponent>();

			////Create 10 entities that follow the ball at different speeds
			//for (size_t i = 0; i < 10; i++) {
			//	entityx::Entity ent3 = entities.create();
			//	auto spriteComp2 = ent3.assign<sf::Sprite>().get();
			//	spriteComp2->setPosition(i * 100, i * 100 % 400);
			//	spriteComp2->setTexture(TextureHandler::getInstance().getTexture("default.png"));

			//	ent3.assign<PaddelComponent>();

			//	auto targetComp = ent3.assign<TargetingComponent>().get();
			//	targetComp->target = ent2;
			//	targetComp->speed = 35 + i * 18;
			//}

			////Create an animated fire entity that does  follow the ball
			//spriteComp3->setPosition(1380, 370);
			//spriteComp3->setTexture(TextureHandler::getInstance().getTexture("../Resources/fireSprite.png"));
			//spriteComp3->setScale(0.5f, 0.5f);
			//spriteComp3->setOrigin(18,86);
			//ent4.assign<AnimationComponent>();

			//auto targetComp = ent4.assign<TargetingComponent>().get();
			//targetComp->target = ent2;
			//targetComp->speed = 10000.0f;

			////Create Opponent
			//entityx::Entity opponentEntity = entities.create();//Store the player entity in m_playerEntity for future use
			//auto opponentEntitySprite = opponentEntity.assign<sf::Sprite>().get();
			//opponentEntitySprite->setTexture(TextureHandler::getInstance().getTexture("../Resources/paddel.png"));
			//opponentEntitySprite->setPosition(m_game->getWindow()->getSize().x - 25 - comp->getGlobalBounds().width, m_game->getWindow()->getSize().y / 2.0f - comp->getGlobalBounds().height / 2.0f);
			//opponentEntitySprite->setColor(sf::Color::Red);
			//auto padcomp2 = opponentEntity.assign<PaddelComponent>();
			//padcomp2->playerID = 1;
			//auto aicomp = opponentEntity.assign<AIComponent>();
			////aicomp->ball = ent2;
		}
		

		// text
		//{
		//	entityx::Entity testtextEntity = entities.create();
		//	auto textcomp = testtextEntity.assign<TextComponent>().get();
		//	textcomp->text.setFont(m_game->GetFont());
		//	textcomp->text.setString("Player 1");
		//	textcomp->text.setPosition(m_game->getWindow()->getSize().x / 4.0f - textcomp->text.getGlobalBounds().width / 2, 0.0f);

		//	systems.system<TextSystem>().get()->setPlayerText(testtextEntity, 0);
		//}
		//{
		//	entityx::Entity testtextEntity = entities.create();
		//	auto textcomp = testtextEntity.assign<TextComponent>().get();
		//	textcomp->text.setFont(m_game->GetFont());
		//	textcomp->text.setString("Player 2");
		//	textcomp->text.setPosition(m_game->getWindow()->getSize().x *3 / 4.0f - textcomp->text.getGlobalBounds().width / 2, 0.0f);

		//	systems.system<TextSystem>().get()->setPlayerText(testtextEntity, 1);
		//}

		//{
		//	//Create the animated character entity
		//	entityx::Entity ent2 = entities.create();
		//	auto spriteComp = ent2.assign<sf::Sprite>().get();
		//	spriteComp->setPosition(500, 650);
		//	spriteComp->setTexture(TextureHandler::getInstance().getTexture("../Resources/character.png"));
		//	auto animationComp = ent2.assign<AnimationComponent>();
		//	animationComp->m_animationSpeed = 16;
		//}
	}
}
