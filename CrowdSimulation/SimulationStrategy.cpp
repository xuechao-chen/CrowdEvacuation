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

	std::unordered_map<glm::vec2, glm::vec2, HashFunc4Node> TempRoadMap;
	const auto& AllNodes = pGraph->dumpAllNodes();
	for (auto& Node : AllNodes)
	{
		if (__isVisited(Node, VisitedNodeSet)) continue;

		//TODO: 针对节点是出口的情况
		//目前的处理，出口的导航点无穷远
		const auto& ShortestPath = __findShortestPathToExit(Node, Exits, pGraph);
		for (auto& NavNode : ShortestPath) VisitedNodeSet.push_back(NavNode);
		
		auto PathSize = ShortestPath.size();
		for (size_t i = 0; i < PathSize - 1; i++)
		{
			auto& CurNavNode = ShortestPath[i];
			auto& NextNavNode = ShortestPath[i + 1];
			TempRoadMap[CurNavNode] = NextNavNode;
			CSimNode* pSimNode = new CSimNode();
			pSimNode->setNodeType(ESimNodeType::NormalNode);
			pSimNode->addNavNode(NextNavNode);
			m_RoadMap[CurNavNode] = pSimNode;
		}
	}

	const auto& AllEdges = pGraph->dumpAllEdges();
	for (auto& Edge : AllEdges)
	{
		auto& Node1 = Edge.first; auto& Node2 = Edge.second;
		if (TempRoadMap[Node1] != Node2 && TempRoadMap[Node2] != Node1)
		{
			//TODO: add divede node
		}
	}

}

void CSimulationStrategy::__assignNavNode2Agent()
{
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
	
}

bool CSimulationStrategy::__isDistributionNodeConverged()
{
}