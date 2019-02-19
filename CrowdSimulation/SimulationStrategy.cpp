#include "SimulationStrategy.h"
#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include "common/CommonInterface.h"
#include <boost/format.hpp>

bool CSimulationStrategy::__isFinish()
{
	return m_IterationNum >= 10;
}

void CSimulationStrategy::__onPostDoStep()
{
	__updateAgentsVelocity();

	if (__isAllAgentReachExit())
	{
		m_IterationNum++;
		__updateScene();
		std::cout << "Evacuation Iteration " << m_IterationNum << " : " << m_EvacuationTimeCost << std::endl;
		m_EvacuationTimeCost = 0;
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
		for (auto& NavNode : ShortestPath.first) VisitedNodeSet.push_back(NavNode);
		__addShortestPath2RoadMap(ShortestPath.first);
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
		CSimNode* pSimNode = new CSimNode(CurNavNode, ESimNodeType::NormalNode);
		pSimNode->addNavNode(NextNavNode);
		m_RoadMap[CurNavNode] = pSimNode;
	}
	CSimNode* pSimNode = new CSimNode(vShortestPath[PathSize - 1], ESimNodeType::NormalNode);
	pSimNode->addNavNode(glm::vec2(FLT_MAX, FLT_MAX));
	m_RoadMap[vShortestPath[PathSize - 1]] = pSimNode;
}

bool CSimulationStrategy::__isAllAgentReachExit()
{
	const auto& Agents = m_pScene->getAgents();
	const auto& Exits = m_pScene->getExits();
	bool IsAllAgentReachExit = true;

	for (auto& Agent : Agents)
	{
		if (!Agent->isReachExit(Exits)) IsAllAgentReachExit = false;
		else {
			Agent->setPosition(glm::vec2(500, 500));
			Agent->setPrefVelocity(glm::vec2(0, 0));
			Agent->setEvacuationTime(m_EvacuationTimeCost);
			Agent->tagIsReachExit(true);
		}
	}
	return IsAllAgentReachExit;
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
				float Rand = rand() % 100 / (float)(101); // 0到1的随机数
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
	//NOTE: 先更新分流点，后更新分割点
	auto TempRoadMap = m_RoadMap;
	for (auto& Item : TempRoadMap)
	{
		auto SimNode = Item.second;
		if (SimNode->getNodeType() == ESimNodeType::DistributionNode) __updateDistributionNode(SimNode);
	}
	for (auto& Item : TempRoadMap)
	{
		auto SimNode = Item.second;
		if (SimNode->getNodeType() == ESimNodeType::DivideNode) __updateDivideNode(SimNode);
	}
	__resetAgents();
}

void CSimulationStrategy::__updateDivideNode(CSimNode* pSimNode)
{
	const auto& NavNode1 = pSimNode->getNavNodeAt(0);
	const auto& NavNode2 = pSimNode->getNavNodeAt(1);
	const auto& DividePos = pSimNode->getPos();
	
	const auto& AgentsInNode1 = m_pScene->dumpAgentsInNode(NavNode1);
	const auto& AgentsInNode2 = m_pScene->dumpAgentsInNode(NavNode2);
	const auto& AgentsInEdge1 = m_pScene->dumpAgentsInEdge(NavNode1, DividePos);
	const auto& AgentsInEdge2 = m_pScene->dumpAgentsInEdge(DividePos, NavNode2);

	auto T1 = __calAvgEvacuationTime4Agents(AgentsInNode1);
	auto T2 = __calAvgEvacuationTime4Agents(AgentsInNode2);
	auto DeltaT1 = __calMaxEvacuationTime4Agents(AgentsInEdge1);
	auto DeltaT2 = __calMaxEvacuationTime4Agents(AgentsInEdge2);
	auto Alpha = DeltaT1 / pSimNode->getDivideRatio();
	auto Beta = DeltaT2 / (1 - pSimNode->getDivideRatio());

	auto UpdatedDivideRatio = (T2 - T1 + Beta) / (Alpha + Beta);
	auto Graph = m_pScene->getGraph();
	if (isnan(UpdatedDivideRatio)) UpdatedDivideRatio = 0.5f;
	if (UpdatedDivideRatio <= 0.05)
	{
		// NavNode1成为分流点
		auto pSimNode = m_RoadMap[NavNode1];
		pSimNode->setNodeType(ESimNodeType::DistributionNode);
		pSimNode->addNavNode(NavNode2);
		pSimNode->addDistributionRatio(0.5);
		pSimNode->addDistributionRatio(0.5);
		for (auto Iter = m_RoadMap.begin(); Iter != m_RoadMap.end(); ++Iter)
		{
			if (Iter->first == DividePos)
			{
				m_RoadMap.erase(Iter); break;
			}
		}
		Graph->removeNode(DividePos);
		Graph->addEdge(NavNode1, NavNode2, glm::distance(NavNode1, NavNode2));
	}
	else if (UpdatedDivideRatio >= 0.95)
	{
		// NavNode2成为分流点
		auto pSimNode = m_RoadMap[NavNode2];
		pSimNode->setNodeType(ESimNodeType::DistributionNode);
		pSimNode->addNavNode(NavNode1);
		pSimNode->addDistributionRatio(0.5);
		pSimNode->addDistributionRatio(0.5);
		for (auto Iter = m_RoadMap.begin(); Iter != m_RoadMap.end(); ++Iter)
		{
			if (Iter->first == DividePos)
			{
				m_RoadMap.erase(Iter); break;
			}
		}
		Graph->removeNode(DividePos);
		Graph->addEdge(NavNode1, NavNode2, glm::distance(NavNode1, NavNode2));
	}
	else
	{
		pSimNode->setDivideRatio(UpdatedDivideRatio);
		glm::vec2 UpdatedDividePos = NavNode1 + (NavNode2 - NavNode1) * UpdatedDivideRatio;
		pSimNode->setPos(UpdatedDividePos);
		for (auto Iter = m_RoadMap.begin(); Iter != m_RoadMap.end(); ++Iter)
		{
			if (Iter->first == DividePos) { m_RoadMap.erase(Iter); break; }
		}
		m_RoadMap[UpdatedDividePos] = pSimNode;
		Graph->removeNode(DividePos);
		Graph->addNode(UpdatedDividePos);
		Graph->addEdge(NavNode1, UpdatedDividePos, glm::distance(NavNode1, UpdatedDividePos));
		Graph->addEdge(UpdatedDividePos, NavNode2, glm::distance(NavNode2, UpdatedDividePos));
	}
}

void CSimulationStrategy::__updateDistributionNode(CSimNode* pSimNode)
{
	std::vector<float> TimeSet;
	for (size_t i = 0; i < pSimNode->getNavNodeNum(); i++)
	{
		const auto& NavNode = pSimNode->getNavNodeAt(i);
		const auto& Agents = m_pScene->dumpAgentsInEdge(pSimNode->getPos(), NavNode);//TODO 边的范围不包括Intersection
		auto MaxTime = __calMaxEvacuationTime4Agents(Agents);
		TimeSet.push_back(1/MaxTime);
	}
	float Total = std::accumulate(TimeSet.begin(), TimeSet.end(), 0.0f);
	for (size_t i = 0; i < pSimNode->getNavNodeNum(); i++)
	{
		pSimNode->setDistributionRatioAt(i, TimeSet[i] / Total);
	}
}

void CSimulationStrategy::__resetAgents()
{
	const auto& Agents = m_pScene->getAgents();
	for (auto Agent : Agents)
	{
		Agent->reset();
	}
	__assignNavNode2Agent();
}

void CSimulationStrategy::__assignNavNode2Agent()
{
	const auto& Agents = m_pScene->getAgents();
	auto Graph = m_pScene->getGraph();
	for (auto& Agent : Agents)
	{
		const auto& NavNodes = Graph->dumpNavNodes(Agent->getPosition());
		if (NavNodes.size() == 1) // Agent on Node
		{
			auto SimNode = m_RoadMap[NavNodes[0]];
			switch (SimNode->getNodeType())
			{
			case ESimNodeType::DivideNode:
			{
				_ASSERTE(SimNode->getNavNodeNum() == 2);
				const auto& NavNode1 = SimNode->getNavNodeAt(0);
				const auto& NavNode2 = SimNode->getNavNodeAt(1);
				auto Distance1 = Graph->getEdgeWeight(SimNode->getPos(), NavNode1);
				if (glm::distance(Agent->getPosition(), NavNode1) < Distance1) Agent->setNavNode(NavNode1);
				else Agent->setNavNode(NavNode2);
				break;
			}
			case ESimNodeType::DistributionNode:
			{
				float Rand = rand() % 100 / (float)(101); // 0到1的随机数
				int NavNodeNum = SimNode->getNavNodeNum();
				auto AccumulatedRatio = 0.0f;
				for (size_t i = 0; i < NavNodeNum; i++)
				{
					auto Ratio = SimNode->getDistributionRatioAt(i);
					AccumulatedRatio += Ratio;
					if (Rand <= AccumulatedRatio)
					{
						Agent->setNavNode(SimNode->getNavNodeAt(i)); break;
					}
				}
				break;
			}
			case ESimNodeType::NormalNode:
			{
				_ASSERTE(SimNode->getNavNodeNum() == 1);
				Agent->setNavNode(SimNode->getNavNodeAt(0));
				break;
			}
			default: break;
			}
		}
		if (NavNodes.size() == 2) // Agent on Edge
		{
			auto& Node1 = NavNodes[0]; auto& Node2 = NavNodes[1];
			auto SimNode1 = m_RoadMap[Node1]; auto SimNode2 = m_RoadMap[Node2];
			switch (SimNode1->getNodeType())
			{
				case ESimNodeType::DivideNode:
				{
					Agent->setNavNode(Node2); break;
				}
				case ESimNodeType::DistributionNode:
				{
					Agent->setNavNode(Node1);
					for (size_t i = 0; i < SimNode1->getNavNodeNum(); i++)
					{
						if (SimNode1->getNavNodeAt(i) == Node2)
						{
							Agent->setNavNode(Node2); break;
						}
					}
					break;
				}
				case ESimNodeType::NormalNode:
				{
					if (SimNode1->getNavNodeAt(0) == Node2) Agent->setNavNode(Node2);
					else Agent->setNavNode(Node1);
					break;
				}
				default:
				{
					_ASSERTE(false);
					break;
				}
			}
		}	
		auto Direcition = Agent->getNavNode() - Agent->getPosition();
		auto Normal = RVO::normalize(RVO::Vector2(Direcition.x, Direcition.y));
		Agent->setPrefVelocity({ Normal.x(), Normal.y() });
	}
}