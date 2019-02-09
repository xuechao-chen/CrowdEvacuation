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
	//TODO: update prefvelocity

	auto IsAllAgentReachExit = true;
	const auto& Agents = m_pScene->getAgents();
	const auto& Exits = m_pScene->getExits();
	for (auto& Agent : Agents)
	{
		if (!Agent->isReachExit(Exits))
		{
			IsAllAgentReachExit = false;
			break;
		}
	}

	if (IsAllAgentReachExit)
	{
		m_IterationNum++;
		__analyzeConvergence();
		if (!m_IsConverged)
		{
			//TODO: reset agent
			//TODO: update scene
			
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
	
			CSimNode* pDivideNode = new CSimNode();
			pDivideNode->setNodeType(ESimNodeType::DivideNode);
			pDivideNode->addNavNode(Node1);
			pDivideNode->addNavNode(Node2);
			pDivideNode->setPos(Pos);

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
				//Agent->
			}
			else
			{   // Normal Node
				_ASSERTE(SimNode->getNavNodeNum() == 1);
				Agent->setNavNode(SimNode->getNavNodeAt(0));
			}
		}
	}
}

void CSimulationStrategy::__analyzeConvergence()
{
	if (__isDivideNodeConverged() && __isDistributionNodeConverged())
	{
		m_IsConverged = true;
	}
}

bool CSimulationStrategy::__isDivideNodeConverged()
{
	return false;
}

bool CSimulationStrategy::__isDistributionNodeConverged()
{
	return false;
}