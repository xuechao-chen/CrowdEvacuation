#include "BaseStrategy.h"
#include "ConfigParser.h"
#include "SceneVis.h"

IEvacuationStrategy::IEvacuationStrategy()
{
}

IEvacuationStrategy::~IEvacuationStrategy()
{
	_SAFE_DELETE(m_pScene);
}

void IEvacuationStrategy::run()
{
	auto pSim = m_pScene->getSimulator();

	std::cout << "Evacuation Begin" << std::endl;
	do {
		m_EvacuationTimeCost++;
		__updateVisualization();
  		pSim->doStep();
		__afterSimulationDoStep();
		__updateAgentVelocity();
	} while (!__isFinish());

	std::cout << "Evacuation End" << std::endl;
	std::cout << "Evacuation time cost: " << m_EvacuationTimeCost << std::endl;
}

void IEvacuationStrategy::init()
{
	__constructEvacuationScene();
	__constructRoadMap();
	__assignNavNode2Agent();
}

bool IEvacuationStrategy::__isFinish()
{
	return _isAllAgentReachExit();
}

void IEvacuationStrategy::__constructRoadMap()
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
		__addPath2RoadMap(ShortestPath);
	}
}

void IEvacuationStrategy::__assignNavNode2Agent()
{
	const auto& Agents = m_pScene->getAgents();
	auto Graph = m_pScene->getGraph();
	for (auto& Agent : Agents)
	{
		const auto& NavNodes = Graph->dumpNavNodes(Agent->getPosition());
		if (NavNodes.size() == 1) Agent->setNavNode(m_RoadMap[NavNodes[0]].first);
		if (NavNodes.size() == 2)
		{
			auto& Node1 = NavNodes[0]; auto& Node2 = NavNodes[1];
			if (m_RoadMap[Node1].first == Node2)      Agent->setNavNode(Node2);
			else if (m_RoadMap[Node2].first == Node1) Agent->setNavNode(Node1);
			else {
				auto Distance1 = glm::distance(Agent->getPosition(), Node1);
				auto Distance2 = glm::distance(Agent->getPosition(), Node2);
				if (m_RoadMap[Node1].second + Distance1 < m_RoadMap[Node2].second + Distance2) Agent->setNavNode(Node1);
				else Agent->setNavNode(Node2);
			}
		}
		auto Direction = Agent->getNavNode() - Agent->getPosition();
		Agent->setPrefVelocity(glm::normalize(Direction));
	}
}

void IEvacuationStrategy::__constructEvacuationScene()
{
	CSceneGraph* pGraph = new CSceneGraph();
	RVO::RVOSimulator* pSim = new RVO::RVOSimulator();

	CConfigParser::parseGraph("GraphConfig.xml", pGraph);
	CConfigParser::parseRVOSimulator("SimulatorConfig.xml", pSim);

	CEvacuationScene* pScene = new CEvacuationScene(pSim, pGraph);
	CConfigParser::parseScene("SceneConfig.xml", pScene);

	setEvacuationScene(pScene);
}

void IEvacuationStrategy::__updateVisualization()
{
	CSceneVis::getInstance()->displayScene(m_pScene);
}

void IEvacuationStrategy::__addPath2RoadMap(const std::pair<std::vector<glm::vec2>, float>& vPath)
{
	auto& NavNodeSet = vPath.first;
	auto Distance = vPath.second;
	auto PathSize = NavNodeSet.size();
	for (size_t i = 0; i < PathSize - 1; i++)
	{
		auto& CurNavNode = NavNodeSet[i];
		auto& NextNavNode = NavNodeSet[i + 1];
		m_RoadMap[CurNavNode].first = NextNavNode;
		m_RoadMap[CurNavNode].second = Distance;
		Distance -= glm::distance(NextNavNode, CurNavNode);
	}
	m_RoadMap[NavNodeSet[PathSize - 1]].first = glm::vec2(FLT_MAX, FLT_MAX);//NOTE: 出口的下一个导航点默认为无限远
	m_RoadMap[NavNodeSet[PathSize - 1]].second = 0;
}

void IEvacuationStrategy::__updateAgentVelocity()
{
	auto Exits = m_pScene->getExits();
	for (auto Agent : m_pScene->getAgents())
	{
		if (!Agent->isReachExit(Exits))
		{
			auto NeighborNum = m_pScene->dumpAgentNeighborNum(Agent);
			auto Factor = 1 - (NeighborNum / 80.0f);
			if (Factor < 0.4) Factor = 0.4;
			glm::vec2 Direction = Agent->getNavNode() - Agent->getPosition();
			Agent->setPrefVelocity(glm::normalize(Direction)*Factor);
		}
	}
}

bool IEvacuationStrategy::_isAllAgentReachExit()
{
	const auto& Agents = m_pScene->getAgents();
	const auto& Exits = m_pScene->getExits();
	bool IsAllAgentReachExit = true;

	for (auto& Agent : Agents)
	{
		if (!Agent->isReachExit(Exits)) IsAllAgentReachExit = false;
		else {
			Agent->setPosition({ -rand() % (100 + 1)-100, -rand() % (100 + 1)-100 });
			Agent->setPrefVelocity(glm::vec2(0, 0));
			Agent->setEvacuationTime(m_EvacuationTimeCost);
			Agent->tagIsReachExit(true);
		}
	}
	return IsAllAgentReachExit;
}

bool __isVisited(const glm::vec2& vNode, const std::vector<glm::vec2>& vVisitedNodeSet)
{
	for (auto& VisitedNode : vVisitedNodeSet)
	{
		if (vNode == VisitedNode) return true;
	}
	return false;
}

std::pair<std::vector<glm::vec2>, float> __findShortestPathToExit(const glm::vec2& vNode, const std::vector<glm::vec2>& vExits, CSceneGraph* pGraph)
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
	return { ShortestNavNodeSet, ShortestDistance };
}

float __calAvgEvacuationTime4Agents(const std::vector<IAgent*>& vAgents)
{
	float TotalTime = 0.0f;
	for (auto Agent : vAgents)
	{
		TotalTime += Agent->getEvacuationTime();
	}
	return TotalTime / vAgents.size();
}

float __calMaxEvacuationTime4Agents(const std::vector<IAgent*>& vAgents)
{
	float MaxTime = -FLT_MAX;
	for (auto Agent : vAgents)
	{
		auto EvacuationTime = Agent->getEvacuationTime();
		if (EvacuationTime > MaxTime) MaxTime = EvacuationTime;
	}
	return MaxTime;
}
