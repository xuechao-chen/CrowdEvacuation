#include "ShortestStrategy.h"

CShortestStrategy::CShortestStrategy()
{
	__init();
}

CShortestStrategy::~CShortestStrategy()
{
}

bool __isVisited(const glm::vec2& vNode, const std::vector<glm::vec2>& vVisitedNodeSet) 
{
	for (auto& VisitedNode : vVisitedNodeSet)
	{
		if (vNode == VisitedNode) return true;
	}
	return false;
}

std::vector<glm::vec2> __findShortestPathToExit(const glm::vec2& vNode, const std::vector<glm::vec2>& vExits, CSceneGraph* pGraph)
{
	std::vector<glm::vec2> ShortestNavNodeSet;
	auto ShortestDistance = FLT_MAX;
	for (auto& Exit : vExits)
	{
		const auto& Path = pGraph->findShortestPath(vNode, Exit);
		auto& NavNodeSet = Path.first;
		auto Distance = Path.second;
		if (Distance < ShortestDistance)
		{
			ShortestNavNodeSet = NavNodeSet;
			ShortestDistance = Distance;
		}
	}
	return ShortestNavNodeSet;
}

void CShortestStrategy::__add2NavNodeMap(const std::vector<glm::vec2>& vShortestPath)
{
	for (size_t i = 0; i < vShortestPath.size()-1; i++)
	{
		auto& CurNavNode = vShortestPath[i];
		auto& NextNavNode = vShortestPath[i+1];
		m_NavNodeMap[CurNavNode] = NextNavNode;
	}
	m_NavNodeMap[*vShortestPath.end()] = glm::vec2(FLT_MAX, FLT_MAX);
}

void CShortestStrategy::__assignNavNode2Agent()
{
	const auto& Agents = m_pScene->getAgents();
	auto Graph = m_pScene->getGraph();
	for (auto& Agent : Agents)
	{
		const auto& NavNodes = Graph->dumpNavNodes(Agent->getPosition());
		if (NavNodes.size() == 1) Agent->setNavNode(m_NavNodeMap[NavNodes[0]]);
		if (NavNodes.size() == 2)
		{
			auto& Node1 = NavNodes[0]; auto& Node2 = NavNodes[1];
			if (m_NavNodeMap[Node1] == Node2)      Agent->setNavNode(Node2);
			else if (m_NavNodeMap[Node2] == Node1) Agent->setNavNode(Node1);
			else {
				auto Distance1 = glm::distance(Agent->getPosition(), Node1);
				auto Distance2 = glm::distance(Agent->getPosition(), Node2);
				if (Distance1 < Distance2) Agent->setNavNode(Node1);
				else Agent->setNavNode(Node2);
			}
		}
	}
}

void CShortestStrategy::__init()
{
	__constructNavNodeMap();
	__assignNavNode2Agent();
}

void CShortestStrategy::__constructNavNodeMap()
{
	auto pGraph = m_pScene->getGraph();
	const auto& AllEdges = pGraph->dumpAllEdges();
	const auto& Exits = m_pScene->getExits();
	auto VisitedNodeSet = std::vector<glm::vec2>();

	const auto& AllNodes = pGraph->dumpAllNodes();
	for (auto& Node : AllNodes)
	{
		if (__isVisited(Node, VisitedNodeSet)) continue;

		//TODO: 针对节点是出口的情况
		const auto& ShortestPath = __findShortestPathToExit(Node, Exits, pGraph);
		for (auto& NavNode : ShortestPath) VisitedNodeSet.push_back(NavNode);
		__add2NavNodeMap(ShortestPath);
	}
}

bool CShortestStrategy::__isFinish()
{
	const auto& Exits = m_pScene->getExits();
	const auto& Agents = m_pScene->getAgents();
	for (auto& Agent : Agents)
	{
		if (!Agent->isReachExit(Exits)) return false;
		//TODO: Agent 到达出口时的处理｛移动到最远处｝
	}
	return true;
}

void CShortestStrategy::__onPreDoStep()
{
	//改变导航点
	const auto& Agents = m_pScene->getAgents();
	for (auto& Agent : Agents)
	{
		if (Agent->isReachNavNode())
		{
			//TODO 针对导航点是出口的情况
			const auto& CurNavNode = Agent->getNavNode();
			Agent->setNavNode(m_NavNodeMap[CurNavNode]);
		}
	}
}

void CShortestStrategy::__onPostDoStep()
{
}