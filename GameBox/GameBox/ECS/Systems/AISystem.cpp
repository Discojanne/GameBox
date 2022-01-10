#include "AISystem.h"
//#include "SFML/Graphics/RenderWindow.hpp"
#include "PickingSystem.h"
#include "CollisionSystem.h"

AISystem::AISystem(sf::RenderWindow* window, entityx::EventManager& events, entityx::EntityManager& es, sf::Vector2i mapDims)
	: m_window(window), m_eventmanager(&events), m_entitymanager(&es), Astar(es, window, mapDims.x, mapDims.y)
{
	m_eventmanager = &events;
	m_eventmanager->subscribe<ClickActionEvent>(*this);

}

AISystem::~AISystem()
{
	m_eventmanager->unsubscribe<ClickActionEvent>(*this);
}

void AISystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	es.each<AIComponent, sf::Sprite>([&](entityx::Entity entity, AIComponent& aicomponent, sf::Sprite& sprite) {

		/*if (aicomponent.ball.valid()) {
			if (aicomponent.ball.has_component<sf::Sprite>()) {
				entityx::ComponentHandle<sf::Sprite> spriteCompHND;
				es.unpack<sf::Sprite>(aicomponent.ball.id(), spriteCompHND);
				

				if (spriteCompHND.get()->getPosition().y < sprite.getPosition().y + sprite.getGlobalBounds().width * 0.4f)
				{
					sprite.move(0, -dt * 600);
					if (sprite.getPosition().y < 0)
					{
						sprite.setPosition(sprite.getPosition().x, 0.0f);
					}
				}
				else if (spriteCompHND.get()->getPosition().y > sprite.getPosition().y - sprite.getGlobalBounds().width * 0.4f)
				{
					sprite.move(0, dt * 600);
					if (sprite.getPosition().y > m_window->getSize().y - sprite.getGlobalBounds().height)
					{
						sprite.setPosition(sprite.getPosition().x, m_window->getSize().y - sprite.getGlobalBounds().height);
					}
				}
			}
		}*/


		sf::Vector2f dir;
		float length;
		sf::Vector2f targPos;

		switch (aicomponent.state)
		{
		case AI::State::IDLE:
			//std::cout << "idle" << std::endl;
			//sprite.setColor(sf::Color::White);
			break;
		case AI::State::WALKING:

			//sprite.setColor(sf::Color::Blue);
			if (aicomponent.targetPosVector.empty())
			{
				aicomponent.state = AI::State::IDLE;
				break;
			}
			
			// Get direction
			targPos = aicomponent.targetPosVector.back();
			dir = targPos - sprite.getPosition();
			// Normalize
			length = sqrtf(dir.x * dir.x + dir.y * dir.y);
			if (length > 5.0f)
			{
				dir.x = dir.x / length;
				dir.y = dir.y / length;
				//std::cout << dir.y << std::endl;
				// Move
				float totalSpeed = (float)(aicomponent.speed * dt);
				//std::cout << totalSpeed << std::endl;

				sprite.move(dir.x * totalSpeed, dir.y * totalSpeed);


				es.each<CollisionComponent, sf::Sprite>([&](entityx::Entity testedEntity, const CollisionComponent& collisionComponent, const sf::Sprite& testedSprite) {

					if (sprite.getGlobalBounds().intersects(testedSprite.getGlobalBounds()) && entity != testedEntity) {
						
						sprite.move(-dir.x * totalSpeed, -dir.y * totalSpeed);
						/*int i = 0;
						do
						{
							switch (i)
							{
							case 0:
								sprite.move(dir.x * totalSpeed, 0);
								break;
							case 1:
								sprite.move(0, dir.y * totalSpeed);
								break;
							case 2:
								sprite.move(-dir.x * totalSpeed, 0);
								break;
							case 3:
								sprite.move(0, -dir.y * totalSpeed);
								break;
							default:
								break;
							}
							sprite.move(-dir.x * totalSpeed, -dir.y * totalSpeed);
							i++;
						} while (sprite.getGlobalBounds().intersects(testedSprite.getGlobalBounds()) && i < 4);*/
						//std::cout << "Collision!!" << std::endl;
					}
					});
			}
			else
			{
				//aicomponent.targetPos = sf::Vector2f(0, 0);
				aicomponent.targetPosVector.pop_back();
				if (aicomponent.targetPosVector.empty())
				{
					aicomponent.state = AI::State::IDLE;
				}
				break;
			}
			

			break;
		default:
			break;
		}



		});

	// Camera movement
	m_entitymanager->each<MapComponent>([&](entityx::Entity entity, MapComponent mapComp) {
		sf::View v = m_window->getView();
		
		if (mapComp.moveLeft)
		{
			if (v.getCenter().x > m_window->getSize().x * 0.4f)
			{
				v.move(-mapComp.moveSpeed * dt, 0);
				m_window->setView(v);
			}
		}
		else if (mapComp.moveRight)
		{
			if (v.getCenter().x < mapComp.width - m_window->getSize().x * 0.4f)
			{
				v.move(mapComp.moveSpeed* dt, 0);
				m_window->setView(v);
			}
		}

		if (mapComp.moveUp)
		{
			if (v.getCenter().y > m_window->getSize().y * 0.4f)
			{
				v.move(0, -mapComp.moveSpeed * dt);
				m_window->setView(v);
			}
		}
		else if (mapComp.moveDown)
		{
			if (v.getCenter().y < mapComp.height - m_window->getSize().y * 0.4f)
			{
				v.move(0, mapComp.moveSpeed* dt);
				m_window->setView(v);
			}
		}

		});
}

void AISystem::receive(const ClickActionEvent& event)
{
	m_entitymanager->each<AIComponent, SelectedComponent>([&](entityx::Entity entity, AIComponent& aicomponent, const SelectedComponent& selectedcomp) {

		auto& comp = *entity.component<AIComponent>().get();
		comp.state = AI::State::WALKING;
		//comp.targetPos = event.mousePos;
		while (!comp.targetPosVector.empty())
		{
			comp.targetPosVector.pop_back();
		}
		
		//comp.targetPosVector.push_back(sf::Vector2f(event.mousePos));
		// 
		//entity.component<sf::Sprite>().get()->setColor(sf::Color::Blue);
		
		
		Astar.Solve_AStar(entity.component<sf::Sprite>().get()->getPosition(), event.mousePos, comp.targetPosVector);

		});
}

void AISystem::tempClickTest(const sf::Vector2f& mousePos)
{
	if (Astar.drawNodes)
	{
		// make sure to hit the circleshape or the value will change without the color...
		float dividerX = Astar.mapWidth / Astar.getWidth();
		float dividerY = Astar.mapHeight / Astar.getHeight();

		int nSelectedNodeX = mousePos.x / dividerX;
		int nSelectedNodeY = mousePos.y / dividerY;

		if (nSelectedNodeX >= 0 && nSelectedNodeX < Astar.getWidth())
		{
			if (nSelectedNodeY >= 0 && nSelectedNodeY < Astar.getHeight())
			{
				Astar.nodes[nSelectedNodeY * Astar.getWidth() + nSelectedNodeX].bObstacle = !Astar.nodes[nSelectedNodeY * Astar.getWidth() + nSelectedNodeX].bObstacle;
				std::cout << Astar.nodes[nSelectedNodeY * Astar.getWidth() + nSelectedNodeX].vecNeighbours.size() << "   " << std::endl;

				m_entitymanager->each<sf::CircleShape>([&](entityx::Entity testedEntity, sf::CircleShape& shape) {

					if (shape.getGlobalBounds().contains(mousePos)) {
						if (Astar.nodes[nSelectedNodeY * Astar.getWidth() + nSelectedNodeX].bObstacle)
						{
							shape.setFillColor(sf::Color::Red);
						}
						else
						{
							shape.setFillColor(sf::Color::Green);
						}
					}
					});
			}
		}
	}
}

void AISystem::DrawNodes()
{
	if (Astar.drawNodes)
	{
		Astar.DrawNodes();
	}
}
