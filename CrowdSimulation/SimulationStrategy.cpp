#include "SimulationStrategy.h"

CSimulationStrategy::CSimulationStrategy()
{
}

CSimulationStrategy::~CSimulationStrategy()
{
}

bool CSimulationStrategy::__isFinish()
{
	if (m_IsConverged || m_IterationNum >= 10)
	{
		return true;
	}
	return false;
}

void CSimulationStrategy::__onPreDoStep()
{
	
}

void CSimulationStrategy::__onPostDoStep()
{
	__updateAgentsVelocity();

	if (__isAllAgentReachExit())
	{
		m_IterationNum++;
		__analyzeConvergence();
		if (!m_IsConverged)
		{
			//TODO: update scene
			//TODO: reset agent
			__updateScene();
		}
	}
}

void CSimulationStrategy::__constructRoadMap()
{
	auto pGraph = m_pScene->getGraph();
	const auto& Exits = m_pScene->getExits();
	auto VisitedNodeSet = std::vector<glm::vec2>();

	const auto& AllNodes = pGraph->dumpAllNodes();
	for (auto& Node : AllNodes)
	{
		if (__isVisited(Node, VisitedNodeSet)) continue;

		const auto& ShortestPath = __findShortestPathToExit(Node, Exits, pGraph);
		for (auto& NavNode : ShortestPath) VisitedNodeSet.push_back(NavNode);
		__addShortestPath2RoadMap(ShortestPath);
	}

	const auto& AllEdges = pGraph->dumpAllEdges();
	for (auto& Edge : AllEdges)
	{
		auto& Node1 = Edge.first; auto& Node2 = Edge.second;
		if (m_RoadMap[Node1]->getNavNodeAt(0) != Node2 &&
			m_RoadMap[Node2]->getNavNodeAt(0) != Node1)
		{
			glm::vec2 Pos((Node1.x + Node2.x) / 2, (Node1.y + Node2.y) / 2);
			pGraph->addNode(Pos);
			pGraph->addEdge(Node1, Pos, glm::distance(Node1, Pos));
			pGraph->addEdge(Pos, Node2, glm::distance(Pos, Node2));
			pGraph->removeEdge(Node1, Node2);
	
			CSimNode* pDivideNode = new CSimNode(Pos, ESimNodeType::DivideNode);
			pDivideNode->addNavNode(Node1);
			pDivideNode->addNavNode(Node2);
			m_RoadMap[Pos] = pDivideNode;
		}
	}
}

void CSimulationStrategy::__addShortestPath2RoadMap(const std::vector<glm::vec2> & vShortestPath)
{
	auto PathSize = vShortestPath.size();
	for (size_t i = 0; i < PathSize - 1; i++)
	{
		auto& CurNavNode = vShortestPath[i];
		auto& NextNavNode = vShortestPath[i + 1];
		CSimNode* pSimNode = new CSimNode();
		pSimNode->setNodeType(ESimNodeType::NormalNode);
		pSimNode->addNavNode(NextNavNode);
		m_RoadMap[CurNavNode] = pSimNode;
	}
	CSimNode* pSimNode = new CSimNode();
	pSimNode->addNavNode(glm::vec2(FLT_MAX, FLT_MAX));
	m_RoadMap[vShortestPath[PathSize - 1]] = pSimNode;
}

bool CSimulationStrategy::__isAllAgentReachExit()
{
	const auto& Agents = m_pScene->getAgents();
	const auto& Exits = m_pScene->getExits();
	for (auto& Agent : Agents)
	{
		if (!Agent->isReachExit(Exits))
		{
			return false;
		}
	}
	return true;
}

void CSimulationStrategy::__updateAgentsVelocity()
{
	const auto& Agents = m_pScene->getAgents();
	for (auto& Agent : Agents)
	{
		if (Agent->isReachNavNode())
		{

			const auto& CurNavNode = Agent->getNavNode();
			glm::vec2 NextNavNode;
			auto SimNode = m_RoadMap[CurNavNode];
			switch (SimNode->getNodeType())
			{
			case ESimNodeType::NormalNode:
			{
				NextNavNode = SimNode->getNavNodeAt(0); break;
			}
			case ESimNodeType::DistributionNode:
			{
				float Rand = rand(); //TODO rand 0~1
				int NavNodeNum = SimNode->getNavNodeNum();
				auto AccumulatedRatio = 0.0f;
				for (size_t i = 0; i < NavNodeNum; i++)
				{
					auto Ratio = SimNode->getDistributionRatioAt(i);
					AccumulatedRatio += Ratio;
					if (Rand <= AccumulatedRatio)
					{
						NextNavNode = SimNode->getNavNodeAt(i);
					}
				}
				break;
			}
			case ESimNodeType::DivideNode:
			default:
				break;
			}

			glm::vec2 Direction;
			if (NextNavNode == glm::vec2(FLT_MAX, FLT_MAX))
			{
				Direction = CurNavNode - Agent->getPosition();
			}
			else
			{   //NOTE: 当前导航点不是出口，更新下一个导航点
				Direction = NextNavNode - Agent->getPosition();
				Agent->setNavNode(NextNavNode);
			}

			auto Normal = RVO::normalize(RVO::Vector2(Direction.x, Direction.y));
			Agent->setPrefVelocity({ Normal.x(), Normal.y() });
		}
	}
}

void CSimulationStrategy::__updateScene()
{
	
}

void CSimulationStrategy::__assignNavNode2Agent()
{
	const auto& Agents = m_pScene->getAgents();
	auto Graph = m_pScene->getGraph();
	for (auto& Agent : Agents)
	{
		const auto& NavNodes = Graph->dumpNavNodes(Agent->getPosition());
		if (NavNodes.size() == 1)
		{
			auto SimNode = m_RoadMap[NavNodes[0]];
			if (SimNode->getNodeType() == ESimNodeType::DivideNode)
			{
				_ASSERTE(SimNode->getNavNodeNum() == 2);
				const auto& NavNode1 = SimNode->getNavNodeAt(0);
				const auto& NavNode2 = SimNode->getNavNodeAt(1);
				auto Distance1 = Graph->getEdgeWeight(SimNode->getPos(), NavNode1);
				if (NavNode1.x == NavNode2.x)
				{
					if (abs(Agent->getPosition().y - NavNode1.y) < Distance1) Agent->setNavNode(NavNode1);
					else	 Agent->setNavNode(NavNode2);
				}
				else
				{
					if (abs(Agent->getPosition().x - NavNode1.x) < Distance1) Agent->setNavNode(NavNode1);
					else Agent->setNavNode(NavNode2);
				}
			}
			else
			{   // Normal Node
				_ASSERTE(SimNode->getNavNodeNum() == 1);
				Agent->setNavNode(SimNode->getNavNodeAt(0));
			}
		}
		if (NavNodes.size() == 2)
		{
			auto& Node1 = NavNodes[0]; auto& Node2 = NavNodes[1];
			auto SimNode1 = m_RoadMap[Node1]; auto SimNode2 = m_RoadMap[Node2];
			if      (SimNode1->getNodeType()   == ESimNodeType::DivideNode)  Agent->setNavNode(Node2);
			else if (SimNode2->getNodeType()   == ESimNodeType::DivideNode)  Agent->setNavNode(Node1);
			else if (SimNode1->getNavNodeAt(0) == Node2) Agent->setNavNode(Node2);
			else if (SimNode2->getNavNodeAt(0) == Node1) Agent->setNavNode(Node1);
			else    _ASSERTE(false);
		}	
		auto Direcition = Agent->getNavNode() - Agent->getPosition();
		auto Normal = RVO::normalize(RVO::Vector2(Direcition.x, Direcition.y));
		Agent->setPrefVelocity({ Normal.x(), Normal.y() });
	}
}

void CSimulationStrategy::__analyzeConvergence()
{
	for (auto& Item : m_RoadMap)
	{
		auto SimNode = Item.second;
		switch (SimNode->getNodeType())
		{
		case ESimNodeType::DivideNode:
		case ESimNodeType::DistributionNode:
		{
			if (!SimNode->isConverged())
			{
				m_IsConverged = false;
				return;
			}
			break;
		}
		default: break;
		}
	}
	m_IsConverged = true;
}