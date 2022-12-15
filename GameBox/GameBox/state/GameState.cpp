#include "GameState.h"
#include "../Game.h"
#include "SFML/Graphics.hpp"
#include <unordered_map>

#include "../ECS/Systems.h"

GameState::GameState(Game* pGame) : State(States::Game, pGame), m_game(pGame) {

	initializeSystems();
	initializeAnimations();
	initializeEntities();

	sf::View tmpView = m_game->getWindow()->getView();
	tmpView.zoom(1.25f);
	m_game->getWindow()->setView(tmpView);
}

GameState::~GameState() {
	
}

void GameState::update(float dt) {

	systems.update<AnimationSystem>(dt);
	systems.update<TargetingSystem>(dt);
	systems.update<BallSystem>(dt);
	systems.update<AISystem>(dt);

	
	systems.update<SpriteRenderSystem>(dt);
	systems.update<ShapeRenderSystem>(dt);
	systems.update<TextSystem>(dt);
	
}

void GameState::processInput(float dt) {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		/*auto playerSprite = m_playerEntity.component<sf::Sprite>().get();
		playerSprite->move(0, -dt * 600);
		if (playerSprite->getPosition().y < 0)
		{
			playerSprite->setPosition(playerSprite->getPosition().x, 0.0f);
		}*/
	
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		/*auto playerSprite = m_playerEntity.component<sf::Sprite>().get();
		playerSprite->move(0, dt * 600);
		if (playerSprite->getPosition().y > m_game->getWindow()->getSize().y - playerSprite->getGlobalBounds().height)
		{
			playerSprite->setPosition(playerSprite->getPosition().x, m_game->getWindow()->getSize().y - playerSprite->getGlobalBounds().height);
		}*/
	}


}


void GameState::handleWindowEvent(const sf::Event& windowEvent) {

	sf::Vector2i mousePos = sf::Mouse::getPosition(*m_game->getWindow());
	int treshold = 100;

	switch (windowEvent.type) {
	case sf::Event::EventType::KeyPressed:
		if (windowEvent.key.code == sf::Keyboard::Escape) {
			m_game->SetState(States::MainMenu);
		}
		if (windowEvent.key.code == sf::Keyboard::A) {
			auto translated_pos = m_game->getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*m_game->getWindow()));
			systems.system<AISystem>().get()->tempClickTest(translated_pos);
		}
		if (windowEvent.key.code == sf::Keyboard::D) {
			

			// get the current mouse position in the window
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_game->getWindow());

			// convert it to world coordinates
			sf::Vector2f worldPos = m_game->getWindow()->mapPixelToCoords(pixelPos);
			std::cout << "cords " << worldPos.x << "  " << worldPos.y << std::endl;
		}
		if (windowEvent.key.code == sf::Keyboard::W) {
			sf::View tmpView = m_game->getWindow()->getView();
			tmpView.zoom(1.1f);
			m_game->getWindow()->setView(tmpView);
		}
		if (windowEvent.key.code == sf::Keyboard::S) {
			sf::View tmpView = m_game->getWindow()->getView();
			tmpView.zoom(0.9f);
			m_game->getWindow()->setView(tmpView);
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

	case sf::Event::EventType::MouseMoved:

		//std::cout << "test" << std::endl;


		
		if (mousePos.x < treshold)
		{
			m_mapEntity.component<MapComponent>().get()->moveLeft = true;
		}
		else
		{
			m_mapEntity.component<MapComponent>().get()->moveLeft = false;
		}

		if (mousePos.x > m_game->getWindow()->getSize().x - treshold)
		{
			m_mapEntity.component<MapComponent>().get()->moveRight = true;
		}
		else
		{
			m_mapEntity.component<MapComponent>().get()->moveRight = false;
		}

		if (mousePos.y < treshold)
		{
			m_mapEntity.component<MapComponent>().get()->moveUp = true;
		}
		else
		{
			m_mapEntity.component<MapComponent>().get()->moveUp = false;
		}

		if (mousePos.y > m_game->getWindow()->getSize().y - treshold)
		{
			m_mapEntity.component<MapComponent>().get()->moveDown = true;
		}
		else
		{
			m_mapEntity.component<MapComponent>().get()->moveDown = false;
		}

		break;

	default:
		break;
	}
}

void GameState::initializeSystems() {
	//Setup EntityX Systems
	systems.add<TargetingSystem>();
	systems.add<BallSystem>(m_game->getWindow());

	
	m_mapEntity = entities.create();
	auto spriteComp = m_mapEntity.assign<sf::Sprite>().get();
	spriteComp->setTexture(TextureHandler::getInstance().getTexture("../Resources/testmap1small.png"));
	auto mapComp = m_mapEntity.assign<MapComponent>().get();
	mapComp->width = spriteComp->getGlobalBounds().width;
	mapComp->height = spriteComp->getGlobalBounds().height;

	// Maybe gather map size from menu?
	systems.add<AISystem>(m_game->getWindow(), events, entities, sf::Vector2i(mapComp->width, mapComp->height));

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
			auto ent = entities.create();
			auto spriteComp = ent.assign<sf::Sprite>().get();
			spriteComp->setPosition(25 + 69 * i, m_game->getWindow()->getSize().y / 2.0f - spriteComp->getGlobalBounds().height / 2.0f);
			spriteComp->setTexture(TextureHandler::getInstance().getTexture("../Resources/cubeman.png"));
			spriteComp->setScale(3, 3);
			ent.assign<AIComponent>();
			ent.assign<CollisionComponent>();
		}

	}

	systems.system<AISystem>().get()->DrawNodes();

}
