#pragma once
#include "entityx/entityx.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "../../events/ClickActionEvent.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include <list>

//namespace sf {
//	class RenderWindow;
//}

namespace AI {
	enum State
	{
		IDLE,
		WALKING
	};
}

struct AIComponent
{
	// Ball entity
	AI::State state = AI::State::IDLE;
	sf::Vector2f targetPos;
	float speed = 100.0f;
	float attackspeed = 3.0f;
	std::vector<sf::Vector2f> targetPosVector;
};

class AStarPathfinding
{
public:
	
	AStarPathfinding(entityx::EntityManager& es, sf::RenderWindow* window) 
		: m_entitymanager(&es), m_window(window){
		Create(window);
	};
	~AStarPathfinding() {
		delete[] nodes;

		if (drawNodes) {
			delete[] visualNodes;
		}
	};
	int const getWidth() { return nManWidth; };
	int const getHeight() { return nManHeight; };
	
	

private:
	struct sNode
	{
		bool bObstacle = false;		// Is the node an obstruction?
		bool bVisited = false;		// Have we searched for this noce before?
		float fGlobalGoal;			// Distance to goal so far
		float fLocalGoal;			// Distance to goal if we took the alternative
		int x;						// Nodes position in 2D space
		int y;
		std::vector<sNode*> vecNeighbours;	// Connections to teighbours
		sNode* parent;				// Node connecting to this node that offers sh
	};

	bool drawNodes = true;
	
	int nManWidth = 16;
	int nManHeight = 16; // strange things happens when 8

	sNode* nodeStart = nullptr;
	sNode* nodeEnd = nullptr;

	// to draw the nodes
	entityx::Entity* visualNodes = nullptr;
	entityx::EntityManager* m_entitymanager;
	sf::RenderWindow* m_window;

public:
	sNode* nodes = nullptr;

	void Create(sf::RenderWindow* window) {

		nodes = new sNode[((size_t)nManWidth * nManHeight)];
		float posScaleX = window->getSize().x / nManWidth;
		float posScaleY = window->getSize().y / nManHeight;
		std::cout << posScaleX << "   " << posScaleY << std::endl;
		for (size_t x = 0; x < nManWidth; x++)
		{
			for (size_t y = 0; y < nManHeight; y++)
			{
				nodes[y * nManWidth + x].x = x * posScaleX;
				nodes[y * nManWidth + x].y = y * posScaleY;
				nodes[y * nManWidth + x].bObstacle = false;
				nodes[y * nManWidth + x].parent = nullptr;
				nodes[y * nManWidth + x].bVisited = false;
			}
		}
		if (drawNodes)
		{
			visualNodes = new entityx::Entity[nManWidth * (size_t)nManHeight];
			for (int x = 0; x < nManWidth; x++)
			{
				for (int y = 0; y < nManHeight; y++)
				{
					visualNodes[y * nManWidth + x] = m_entitymanager->create();
					auto shape = visualNodes[y * nManWidth + x].assign<sf::CircleShape>(10.0f).get();
					shape->setPosition(x * posScaleX, y * posScaleY);
					shape->setFillColor(sf::Color::Green);
				}
			}
		}

		// Create connections
		for (int x = 0; x < nManWidth; x++)
		{
			for (int y = 0; y < nManHeight; y++)
			{
				size_t id = (size_t)y * nManWidth + x;
				std::vector<sNode*>& tmpVec = nodes[id].vecNeighbours;
				if (y > 0)
					tmpVec.push_back(&nodes[(y - 1) * nManWidth + (x + 0)]);
				if (y < nManHeight - 1)
					tmpVec.push_back(&nodes[(y + 1) * nManWidth + (x + 0)]);
				if (x > 0)
					tmpVec.push_back(&nodes[(y + 0) * nManWidth + (x - 1)]);
				if (x < nManWidth - 1)
					tmpVec.push_back(&nodes[(y + 0) * nManWidth + (x + 1)]);

				// We can also connect diagonally
				if (y > 0 && x > 0)
					nodes[y* nManWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nManWidth + (x - 1)]);
				if (y < nManHeight - 1 && x > 0)
					nodes[y* nManWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nManWidth + (x - 1)]);
				if (y > 0 && x < nManWidth - 1)
					nodes[y* nManWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nManWidth + (x + 1)]);
				if (y < nManHeight - 1 && x < nManWidth - 1)
					nodes[y* nManWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nManWidth + (x + 1)]);
				

			}
		}

		nodeStart = &nodes[(nManHeight / 2) * nManWidth + 1];
		nodeEnd = &nodes[(nManHeight / 2) * nManWidth + nManWidth - 2];

	};

	void Solve_AStar(sf::Vector2f startPos, sf::Vector2f targetPos, std::vector<sf::Vector2f>& posVector) {


		// Determine start end end node from the start and target postition

		float dividerX = m_window->getSize().x / nManWidth;
		float dividerY = m_window->getSize().y / nManHeight;

		int nSelectedNodeX = startPos.x / dividerX + 0.5f;
		int nSelectedNodeY = startPos.y / dividerY + 0.5f;

		if (nSelectedNodeX >= 0 && nSelectedNodeX < nManWidth)
		{
			if (nSelectedNodeY >= 0 && nSelectedNodeY < nManHeight)
			{
				nodeStart = &nodes[nSelectedNodeY * nManHeight + nSelectedNodeX];
			}
		}
		nSelectedNodeX = targetPos.x / dividerX + 0.5f;
		nSelectedNodeY = targetPos.y / dividerY + 0.5f;

		if (nSelectedNodeX >= 0 && nSelectedNodeX < nManWidth)
		{
			if (nSelectedNodeY >= 0 && nSelectedNodeY < nManHeight)
			{
				nodeEnd = &nodes[nSelectedNodeY * nManHeight + nSelectedNodeX];
			}
		}






		// Reset navigation graph - default all node states
		for (int x = 0; x < nManWidth; x++)
		{
			for (int y = 0; y < nManHeight; y++)
			{
				nodes[y * nManWidth + x].fGlobalGoal = INFINITY;
				nodes[y * nManWidth + x].fLocalGoal = INFINITY;
				nodes[y * nManWidth + x].parent = nullptr;
				nodes[y * nManWidth + x].bVisited = false;
			}
		}

		auto distance = [](sNode* a, sNode* b) // For convenience
		{
			return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
		};

		auto heuristic = [distance](sNode* a, sNode* b)
		{
			return distance(a, b);
		};

		// Setup start conditions
		sNode* nodeCurrent = nodeStart;
		nodeStart->fLocalGoal = 0.0f;
		nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);


		// Add start node to not tested list - this will ensure it gets tested.
		// As the algorithm progresses, newly discovered nodes get added to this
		// list, and will themselves be tested later
		std::list<sNode*> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);

		// if the not tested list contains nodes, there may be better paths
		// which have not yet been explored. However, we will also stop 
		// searching when we reach the target - there may well be better
		// paths but this one will do - it wont be the longest.
		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
		{
			// Sort Untested nodes by global goal, so lowest is first
			listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

			// Front of listNotTestedNodes is potentially the lowest distance node. Our
			// list may also contain nodes that have been visited, so ditch these...
			while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
				listNotTestedNodes.pop_front();

			// ...or abort because there are no valid nodes left to test
			if (listNotTestedNodes.empty())
				break;

			nodeCurrent = listNotTestedNodes.front();
			nodeCurrent->bVisited = true; // We only explore a node once


			// Check each of this node's neighbours...
			for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
			{
				// ... and only if the neighbour is not visited and is 
				// not an obstacle, add it to NotTested List
				if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				// Calculate the neighbours potential lowest parent distance
				float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

				// If choosing to path through this node is a lower distance than what 
				// the neighbour currently has set, update the neighbour to use this node
				// as the path source, and set its distance scores as necessary
				if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
				{
					nodeNeighbour->parent = nodeCurrent;
					nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

					// The best path length to the neighbour being tested has changed, so
					// update the neighbour's score. The heuristic is used to globally bias
					// the path algorithm, so it knows if its getting better or worse. At some
					// point the algo will realise this path is worse and abandon it, and then go
					// and search along the next best path.
					nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
				}
			}
		}

		posVector.clear();
		// Om noder i rad, ta bort noder i mitten av raden
		if (nodeEnd != nullptr)
		{
			sNode* p = nodeEnd;
			while (p->parent != nullptr)
			{
				posVector.push_back(sf::Vector2f(p->x, p->y));
				p = p->parent;
			}
		}
	}
};


class AISystem : public entityx::System<AISystem>, public entityx::Receiver<AISystem>
{
public:
	explicit AISystem(sf::RenderWindow* window, entityx::EventManager& events, entityx::EntityManager& es);
	~AISystem();
	void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

	void receive(const ClickActionEvent& event);

	// used to debug A*
	void tempClickTest(const sf::Vector2f& mousePos);
private:
	sf::RenderWindow* m_window;
	entityx::EntityManager* m_entitymanager;
	entityx::EventManager* m_eventmanager;

	AStarPathfinding Astar;
};