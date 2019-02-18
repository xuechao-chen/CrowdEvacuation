#include "SimulationStrategy.h"
#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include "common/CommonInterface.h"
#include <boost/format.hpp>

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
	__updateAgentsVelocity();

	if (__isAllAgentReachExit())
	{
		m_IterationNum++;
		__updateScene();
		//__analyzeConvergence();
	}
}

void CSimulationStrategy::__constructRoadMap()
{
	__constructBasicRoadMap();

	auto pGraph = m_pScene->getGraph();
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
	//update Divide Node & Distribution Node
	for (auto& Item : m_RoadMap)
	{
		auto SimNode = Item.second;
		if (SimNode->getNodeType() == ESimNodeType::DivideNode) __updateDivideNode(SimNode);
		if (SimNode->getNodeType() == ESimNodeType::DistributionNode) __updateDistributionNode(SimNode);
	}
	__resetAgents();
}

void CSimulationStrategy::__updateDivideNode(CSimNode * pSimNode)
{
	const auto& NavNode1 = pSimNode->getNavNodeAt(0);
	const auto& NavNode2 = pSimNode->getNavNodeAt(1);
	const auto& DividePos = pSimNode->getPos();
	// T_1, T_2
	const auto& AgentsInNode1 = m_pScene->dumpAgentsInNode(NavNode1);
	const auto& AgentsInNode2 = m_pScene->dumpAgentsInNode(NavNode2);
	const auto& AgentsInEdge1 = m_pScene->dumpAgentsInEdge(NavNode1, DividePos);
	const auto& AgentsInEdge2 = m_pScene->dumpAgentsInEdge(NavNode2, DividePos);

	auto T1 = __calAvgEvacuationTime4Agents(AgentsInNode1);
	auto T2 = __calAvgEvacuationTime4Agents(AgentsInNode2);
	auto DeltaT1 = __calMaxEvacuationTime4Agents(AgentsInEdge1);
	auto DeltaT2 = __calMaxEvacuationTime4Agents(AgentsInEdge2);
	auto Alpha = DeltaT1 / pSimNode->getDivideRatio();
	auto Beta = DeltaT2 / (1 - pSimNode->getDivideRatio());

	auto UpdatedDivideRatio = (T2 - T1 + Beta) / (Alpha + Beta);
	if (UpdatedDivideRatio <= 0)
	{
		// NavNode1成为分流点
		auto pSimNode = m_RoadMap[NavNode1];
		pSimNode->setNodeType(ESimNodeType::DistributionNode);
		pSimNode->addNavNode(NavNode2);
		pSimNode->addDistributionRatio(0.5);
		pSimNode->addDistributionRatio(0.5);
		for (auto Iter = m_RoadMap.begin(); Iter != m_RoadMap.end(); ++Iter)
		{
			if (Iter->first == DividePos) m_RoadMap.erase(Iter);
		}
	}
	else if (UpdatedDivideRatio >= 1)
	{
		// NavNode2成为分流点
		auto pSimNode = m_RoadMap[NavNode2];
		pSimNode->setNodeType(ESimNodeType::DistributionNode);
		pSimNode->addNavNode(NavNode1);
		pSimNode->addDistributionRatio(0.5);
		pSimNode->addDistributionRatio(0.5);
		for (auto Iter = m_RoadMap.begin(); Iter != m_RoadMap.end(); ++Iter)
		{
			if (Iter->first == DividePos) m_RoadMap.erase(Iter);
		}
	}
	else
	{
		pSimNode->setDivideRatio(UpdatedDivideRatio);
		pSimNode->updateDividePos();
	}
}

void CSimulationStrategy::__updateDistributionNode(CSimNode * pSimNode)
{
	std::vector<float> TimeSet;
	for (size_t i = 0; i < pSimNode->getNavNodeNum(); i++)
	{
		const auto& NavNode = pSimNode->getNavNodeAt(i);
		const auto& Agents = m_pScene->dumpAgentsInEdge(pSimNode->getPos(), NavNode);//TODO 边的范围不包括Intersection
		auto MaxTime = __calMaxEvacuationTime4Agents(Agents);
		TimeSet.push_back(MaxTime);
	}
	auto Total = std::accumulate(TimeSet.begin(), TimeSet.end(), 0);
	for (size_t i = 0; i < pSimNode->getNavNodeNum(); i++)
	{
		pSimNode->setDistributionRatioAt(i, (1 / TimeSet[i]) / Total);
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

void CSimulationStrategy::__constructRoadMapFromFile()
{
	// reset roadmap to basic state
	m_RoadMap.clear();
	m_RoadMap = m_BasicRoadMap;

	__addDivideNode2RoadMap();
	__addDistributionNode2RoadMap();
}

void CSimulationStrategy::__constructBasicRoadMap()
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
}

void CSimulationStrategy::__addDivideNode2RoadMap()
{
	std::string FileStr = (boost::format("DivideNode_%1%.txt") % m_IterationNum).str();
	std::ifstream DivideNodeFile(FileStr);
	std::string DivideNodeStr;

	while (std::getline(DivideNodeFile, DivideNodeStr))
	{
		int* DivideNodeInfo = new int[6];
		hiveCommon::hiveSplitLine2IntArray(DivideNodeStr, " ", 6 , DivideNodeInfo);
		glm::vec2 DivideNodePos(DivideNodeInfo[0], DivideNodeInfo[1]);
		glm::vec2 NavNode1Pos(DivideNodeInfo[2], DivideNodeInfo[3]);
		glm::vec2 NavNode2Pos(DivideNodeInfo[4], DivideNodeInfo[5]);

		CSimNode* pDivideNode = new CSimNode(DivideNodePos, ESimNodeType::DivideNode);
		pDivideNode->addNavNode(NavNode1Pos);
		pDivideNode->addNavNode(NavNode2Pos);
		m_RoadMap[DivideNodePos] = pDivideNode;
	}
}

void CSimulationStrategy::__addDistributionNode2RoadMap()
{
	std::string FileStr = (boost::format("DistributionNode_%1%.txt") % m_IterationNum).str();
	std::ifstream DistributionNodeFile(FileStr);
	std::string DistributionNodeStr;
	std::string NavNodeStr;
	std::string DistributionRatioStr;

	while (std::getline(DistributionNodeFile, DistributionNodeStr) &&
		   std::getline(DistributionNodeFile, NavNodeStr) &&
		   std::getline(DistributionNodeFile, DistributionRatioStr))
	{
		int* DistributionNodeInfo = new int[3];
		hiveCommon::hiveSplitLine2IntArray(DistributionNodeStr, " ", 3, DistributionNodeInfo);
		glm::vec2 DistributionNodePos(DistributionNodeInfo[0], DistributionNodeInfo[1]);
		int NavNodeNum = DistributionNodeInfo[2];

		CSimNode* pDistributionNode = new CSimNode(DistributionNodePos, ESimNodeType::DistributionNode);

		int* NavNodeInfo = new int[NavNodeNum*2];
		float* RatioInfo = new float[NavNodeNum];
		hiveCommon::hiveSplitLine2IntArray(NavNodeStr, " ", NavNodeNum*2, NavNodeInfo);
		hiveCommon::hiveSplitLine2FloatArray(DistributionRatioStr, " ", NavNodeNum, RatioInfo);
		for (size_t i = 0; i < NavNodeNum; i++)
		{
			glm::vec2 NavNodePos(NavNodeInfo[i*2], NavNodeInfo[i*2+1]);
			pDistributionNode->addNavNode(NavNodePos);
			pDistributionNode->addDistributionRatio(RatioInfo[i]);
			m_RoadMap[DistributionNodePos] = pDistributionNode;
		}
	}
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
				_ASSERTE(SimNode->getNavNodeNum() == 2);
				const auto& NavNode1 = SimNode->getNavNodeAt(0);
				const auto& NavNode2 = SimNode->getNavNodeAt(1);
				auto Distance1 = Graph->getEdgeWeight(SimNode->getPos(), NavNode1);
				if (NavNode1.x == NavNode2.x)
				{
					if (abs(Agent->getPosition().y - NavNode1.y) < Distance1) Agent->setNavNode(NavNode1);
					else Agent->setNavNode(NavNode2);
				}
				else
				{
					if (abs(Agent->getPosition().x - NavNode1.x) < Distance1) Agent->setNavNode(NavNode1);
					else Agent->setNavNode(NavNode2);
				}
			}
			else
			{   // Normal Node
				_ASSERTE(SimNode->getNavNodeNum() == 1);
				Agent->setNavNode(SimNode->getNavNodeAt(0));
			}
		}
		if (NavNodes.size() == 2)
		{
			auto& Node1 = NavNodes[0]; auto& Node2 = NavNodes[1];
			auto SimNode1 = m_RoadMap[Node1]; auto SimNode2 = m_RoadMap[Node2];
			if      (SimNode1->getNodeType()   == ESimNodeType::DivideNode)  Agent->setNavNode(Node2);
			else if (SimNode2->getNodeType()   == ESimNodeType::DivideNode)  Agent->setNavNode(Node1);
			else if (SimNode1->getNavNodeAt(0) == Node2) Agent->setNavNode(Node2);
			else if (SimNode2->getNavNodeAt(0) == Node1) Agent->setNavNode(Node1);
			else    _ASSERTE(false);
		}	
		auto Direcition = Agent->getNavNode() - Agent->getPosition();
		auto Normal = RVO::normalize(RVO::Vector2(Direcition.x, Direcition.y));
		Agent->setPrefVelocity({ Normal.x(), Normal.y() });
	}
}

void CSimulationStrategy::__analyzeConvergence()
{
	for (auto& Item : m_RoadMap)
	{
		auto SimNode = Item.second;
		switch (SimNode->getNodeType())
		{
		case ESimNodeType::DivideNode:
		case ESimNodeType::DistributionNode:
		{
			if (!SimNode->isConverged())
			{
				m_IsConverged = false;
				return;
			}
			break;
		}
		default: break;
		}
	}
	m_IsConverged = true;
}