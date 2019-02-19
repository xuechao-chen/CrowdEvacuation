#include "CDFPredictionStrategy.h"
#include <iostream>

CCDFPredictionStrategy::CCDFPredictionStrategy()
{
}

CCDFPredictionStrategy::~CCDFPredictionStrategy()
{
}

void CCDFPredictionStrategy::init()
{
	IEvacuationStrategy::init();
	__initIntersections();
}

void CCDFPredictionStrategy::__initIntersections()
{
	auto Graph = m_pScene->getGraph();
	for (auto Node : Graph->dumpAllNodes())
	{
		CIntersection* pIntersection = new CIntersection(Node);
		m_Intersections[Node] = pIntersection;
	}
}

bool CCDFPredictionStrategy::__isFinish()
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
			Agent->setPosition({ -rand() % (100 + 1), -rand() % (100 + 1) });
			Agent->setPrefVelocity(glm::vec2(0, 0));
			Agent->setEvacuationTime(m_EvacuationTimeCost);
		}
	}

	return IsFinished;                                                                                                 
}

void CCDFPredictionStrategy::__afterSimulationDoStep()
{
	__updateIntersections();
	__updateAgentsNavigation();
}

void CCDFPredictionStrategy::__constructRoadMap()
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

void CCDFPredictionStrategy::__assignNavNode2Agent()
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

void CCDFPredictionStrategy::__add2RoadMap(const std::vector<glm::vec2>& vShortestPath)
{
	auto PathSize = vShortestPath.size();
	for (size_t i = 0; i < PathSize - 1; i++)
	{
		auto& CurNavNode = vShortestPath[i];
		auto& NextNavNode = vShortestPath[i + 1];
		m_RoadMap[CurNavNode] = NextNavNode;
	}
	m_RoadMap[vShortestPath[PathSize - 1]] = glm::vec2(FLT_MAX, FLT_MAX);
}

void CCDFPredictionStrategy::__updateIntersections()
{
	for (auto Intersection : m_Intersections) Intersection.second->clear();

	for (auto Agent : m_pScene->getAgents())
	{
		if (!Agent->isReachExit(m_pScene->getExits()))
		{
			auto NavNode = Agent->getNavNode();
			auto Speed = Agent->getSpeed();
			auto Distance = glm::distance(Agent->getPosition(), NavNode);
			auto ArrivalTime = Distance / Speed;

			auto Intersection = m_Intersections[NavNode];
			Intersection->addArriveMoment(ArrivalTime, 1.0f); //TODO: 考虑Agent跨边的影响
		}
	}
}

void CCDFPredictionStrategy::__updateAgentsNavigation()
{
	const auto& Agents = m_pScene->getAgents();
	auto Graph = m_pScene->getGraph();
	const auto& Exits = m_pScene->getExits();
	for (auto& Agent : Agents)
	{
		if (Agent->isReachNavNode())
		{
			const auto& CurNavNode = Agent->getNavNode();
			glm::vec2 NextNavNode;

			if (m_RoadMap[CurNavNode] == glm::vec2(FLT_MAX, FLT_MAX)) NextNavNode = CurNavNode;
			else
			{
				const auto& AgentsInCurIntersection = m_pScene->dumpAgentsInNode(CurNavNode);
				auto w1 = __CDF2Factor(AgentsInCurIntersection.size());

				const auto& AdjNodeSet = Graph->dumpAdjNodeSet(CurNavNode);
				auto MinCost = FLT_MAX;
				for (auto& AdjNode : AdjNodeSet)
				{
					auto t1 = w1 * AdjNode.second;
					auto AdjIntersection = m_Intersections[AdjNode.first];
					auto CDF = AdjIntersection->calCDF(t1);
					auto w2 = __CDF2Factor(CDF);
					const auto& ShortestPath = __findShortestPathToExit(AdjNode.first, Exits, Graph);
					auto t2 = w2 * ShortestPath.second;
					auto Cost = t1 + t2;
					if (Cost < MinCost)
					{
						MinCost = Cost;
						NextNavNode = AdjNode.first;
					}
				}
			}
			
			glm::vec2 Direction = NextNavNode - Agent->getPosition();
			Agent->setNavNode(NextNavNode);
			auto Normal = RVO::normalize(RVO::Vector2(Direction.x, Direction.y));
			Agent->setPrefVelocity({ Normal.x(), Normal.y() });
		}
	}
}

float __CDF2Factor(float vCDF)
{
	return pow(2, vCDF / 60);
}