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
			auto Distance2NavNode = glm::distance(Agent->getPosition(), NavNode);
			auto ArrivalTime2NavNode = Distance2NavNode / Speed;

			auto Intersection = m_Intersections[NavNode];
			Intersection->addArriveMoment(ArrivalTime2NavNode, 1.0f); //TODO: 考虑Agent跨边的影响

			auto NextNavNode = m_RoadMap[NavNode].first;
			if (NextNavNode != glm::vec2(FLT_MAX, FLT_MAX))
			{
				auto Distance2NextNavNode = Distance2NavNode + glm::distance(NavNode, NextNavNode);
				auto ArrivalTime2NextNavNode = Distance2NextNavNode / Speed;
				auto NextIntersection = m_Intersections[NextNavNode];
				NextIntersection->addArriveMoment(ArrivalTime2NextNavNode, 1.0f);
			}
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
			if (m_RoadMap[CurNavNode].first == glm::vec2(FLT_MAX, FLT_MAX)) NextNavNode = CurNavNode;
			else
			{
				const auto& AgentsInCurIntersection = m_pScene->dumpAgentsInNode(CurNavNode, false);
				auto w1 = __CDF2Factor(AgentsInCurIntersection.size());
				if (w1 > 1.1) w1 = 1.1;

				const auto& AdjNodeSet = Graph->dumpAdjNodeSet(CurNavNode);
				auto MinCost = FLT_MAX;
				for (auto& AdjNode : AdjNodeSet)
				{
					auto Angle = glm::dot(glm::normalize(AdjNode.first - Agent->getPosition()), glm::normalize(CurNavNode - Agent->getPosition()));
					if (Angle < -0.82) continue;

					auto t1 = w1 * AdjNode.second;
					auto AdjIntersection = m_Intersections[AdjNode.first];
					auto CDF = AdjIntersection->calCDF(t1);
					auto w2 = __CDF2Factor(CDF);
					const auto& ShortestPath = __findShortestPathToExit(AdjNode.first, Exits, Graph);
					auto t2 = w2 * ShortestPath.second;
					auto Cost = t1 + t2;
					if (CurNavNode == glm::vec2(450, 210))
					{
						std::cout << t1 << " " << w1 << " " << t2 << " " << w2 << " " << Cost << std::endl;
					}
					if (Cost < MinCost)
					{
						MinCost = Cost;
						NextNavNode = AdjNode.first;
					}
				}
			}

			// avoid collison
			const auto& AgentsInCurNavNode = m_pScene->dumpAgentsInNode(CurNavNode, false);
			for (auto AgentInCurNavNode : AgentsInCurNavNode)
			{
				if (AgentInCurNavNode->getNavNode() != NextNavNode)
				{
					float a = glm::dot(NextNavNode - Agent->getPosition(), AgentInCurNavNode->getPosition() - Agent->getPosition());
					if (a > 0)
					{
						glm::vec2 SwapNavNode = AgentInCurNavNode->getNavNode();
						AgentInCurNavNode->setNavNode(NextNavNode);
						NextNavNode = SwapNavNode;
						break;
					}
				}
			}

			Agent->setNavNode(NextNavNode);
		}
	}
}

float __CDF2Factor(float vCDF)
{
	auto MaxFactor = CStrategyConfig::getInstance()->getAttribute<float>(KEY_WORDS::MAX_FACTOR);
	auto MaxAgent  = CStrategyConfig::getInstance()->getAttribute<float>(KEY_WORDS::MAX_AGENT);

	return pow(MaxFactor, vCDF / MaxAgent);
}