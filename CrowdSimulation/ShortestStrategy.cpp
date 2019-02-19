#include "ShortestStrategy.h"

CShortestStrategy::CShortestStrategy()
{
}

CShortestStrategy::~CShortestStrategy()
{
}

void CShortestStrategy::__add2RoadMap(const std::vector<glm::vec2>& vShortestPath)
{
	auto PathSize = vShortestPath.size();
	for (size_t i = 0; i < PathSize-1; i++)
	{
		auto& CurNavNode = vShortestPath[i];
		auto& NextNavNode = vShortestPath[i+1];
		m_RoadMap[CurNavNode] = NextNavNode;
	}
	m_RoadMap[vShortestPath[PathSize-1]] = glm::vec2(FLT_MAX, FLT_MAX);
}

void CShortestStrategy::__assignNavNode2Agent()
{
	const auto& Agents = m_pScene->getAgents();
	auto Graph = m_pScene->getGraph();
	for (auto& Agent : Agents)
	{
		const auto& NavNodes = Graph->dumpNavNodes(Agent->getPosition());
		if (NavNodes.size() == 1) Agent->setNavNode(m_RoadMap[NavNodes[0]]);
		if (NavNodes.size() == 2)
		{
			auto& Node1 = NavNodes[0]; auto& Node2 = NavNodes[1];
			if (m_RoadMap[Node1] == Node2)      Agent->setNavNode(Node2);
			else if (m_RoadMap[Node2] == Node1) Agent->setNavNode(Node1);
			else {
				auto Distance1 = glm::distance(Agent->getPosition(), Node1);
				auto Distance2 = glm::distance(Agent->getPosition(), Node2);
				if (Distance1 < Distance2) Agent->setNavNode(Node1);
				else Agent->setNavNode(Node2);
			}
		}
		auto Direcition = Agent->getNavNode() - Agent->getPosition();
		auto Normal = RVO::normalize(RVO::Vector2(Direcition.x, Direcition.y));
		Agent->setPrefVelocity({ Normal.x(), Normal.y() });
	}
}

void CShortestStrategy::__constructRoadMap()
{
	auto pGraph = m_pScene->getGraph();
	const auto& Exits = m_pScene->getExits();
	auto VisitedNodeSet = std::vector<glm::vec2>();

	const auto& AllNodes = pGraph->dumpAllNodes();
	for (auto& Node : AllNodes)
	{
		if (__isVisited(Node, VisitedNodeSet)) continue;

		const auto& ShortestPath = __findShortestPathToExit(Node, Exits, pGraph);
		for (auto& NavNode : ShortestPath.first) VisitedNodeSet.push_back(NavNode);
		__add2RoadMap(ShortestPath.first);
	}
}

bool CShortestStrategy::__isFinish()
{
	const auto& Exits = m_pScene->getExits();
	const auto& Agents = m_pScene->getAgents();
	bool IsFinished = true;

	for (auto& Agent : Agents)
	{
		if (!Agent->isReachExit(Exits)) IsFinished = false;
		else { 
			//NOTE: 到达出口，移动到场景外,速度为0,移到无穷远会触发RVO的建KD树的bug
			//TODO: 从配置文件读取场景长宽来确定位置
			Agent->tagIsReachExit(true);
			Agent->setPosition(glm::vec2(500, 500));
			Agent->setPrefVelocity(glm::vec2(0,0));
			Agent->setEvacuationTime(m_EvacuationTimeCost);
		}
	}

	return IsFinished;
}

void CShortestStrategy::__afterSimulationDoStep()
{
	const auto& Agents = m_pScene->getAgents();
	for (auto& Agent : Agents)
	{
		if (Agent->isReachNavNode())
		{
			const auto& CurNavNode = Agent->getNavNode();
			const auto& NextNavNode = m_RoadMap[CurNavNode];

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