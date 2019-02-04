#include "ShortestStrategy.h"
#include "SceneVis.h"

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
	auto PathSize = vShortestPath.size();
	for (size_t i = 0; i < PathSize-1; i++)
	{
		auto& CurNavNode = vShortestPath[i];
		auto& NextNavNode = vShortestPath[i+1];
		m_NavNodeMap[CurNavNode] = NextNavNode;
	}
	m_NavNodeMap[vShortestPath[PathSize-1]] = glm::vec2(FLT_MAX, FLT_MAX);
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
		auto Direcition = Agent->getNavNode() - Agent->getPosition();
		auto Normal = RVO::normalize(RVO::Vector2(Direcition.x, Direcition.y));
		Agent->setPrefVelocity({ Normal.x(), Normal.y() });
	}
}

void CShortestStrategy::__init()
{
	__constructNavNodeMap();
	__assignNavNode2Agent();
}

bool __belongToExits(const glm::vec2& vNode, const std::vector<glm::vec2>& vExits)
{
	for (auto& Exit : vExits)
	{
		if (vNode == Exit) return true;
	}
	return false;
}

void CShortestStrategy::__constructNavNodeMap()
{
	auto pGraph = m_pScene->getGraph();
	const auto& Exits = m_pScene->getExits();
	auto VisitedNodeSet = std::vector<glm::vec2>();

	const auto& AllNodes = pGraph->dumpAllNodes();
	for (auto& Node : AllNodes)
	{
		if (__isVisited(Node, VisitedNodeSet)) continue;

		//TODO: 针对节点是出口的情况
		//目前的处理，出口的导航点无穷远
		const auto& ShortestPath = __findShortestPathToExit(Node, Exits, pGraph);
		for (auto& NavNode : ShortestPath) VisitedNodeSet.push_back(NavNode);
		__add2NavNodeMap(ShortestPath);
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
			Agent->setPosition(glm::vec2(500, 500));
			Agent->setPrefVelocity(glm::vec2(0,0));
		}
	}

	return IsFinished;
}

void CShortestStrategy::__onPreDoStep()
{
	const auto& Exits = m_pScene->getExits();
	const auto& Agents = m_pScene->getAgents();
	for (auto& Agent : Agents)
	{
		if (Agent->isReachNavNode())
		{
			const auto& CurNavNode = Agent->getNavNode();
			const auto& NextNavNode = m_NavNodeMap[CurNavNode];

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

void CShortestStrategy::__onPostDoStep()
{
	CSceneVis::getInstance()->displayScene(m_pScene);
}