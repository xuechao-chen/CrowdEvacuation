#include "CDFPredictionStrategy.h"
#include "common/ProductFactory.h"
#include "StrategyConfig.h"

hiveOO::CProductFactory<CCDFPredictionStrategy> theCreator(KEY_WORDS::CDF_STRATEGY);

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

void CCDFPredictionStrategy::__afterSimulationDoStep()
{
	//TODO: 考虑边上人的密度影响 updateEdgeWeight()
	__updateIntersections();
	__updateAgentsNavigation();
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
		{ //TODO: 优化，到达导航点之后不需要判断
			const auto& CurNavNode = Agent->getNavNode();
			glm::vec2 NextNavNode;

			//NOTE: 当前导航点为出口，则下个导航点仍为出口
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

			Agent->setNavNode(NextNavNode);
		}
	}
}

float __CDF2Factor(float vCDF)
{
	return pow(2, vCDF / 60);
}