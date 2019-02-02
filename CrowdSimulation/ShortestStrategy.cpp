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
		if (vNode == VisitedNode)
		{
			return true;
		}
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
		m_NavNodeMap[CurNavNode] = new glm::vec2(NextNavNode.x, NextNavNode.y);
	}
	m_NavNodeMap[*vShortestPath.end()] = nullptr;
}

void CShortestStrategy::__assignNavNode2Agent()
{
	//TODO: 根据导航点网络给Agent分配初始导航点

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
	//判断所有agent是否到达出口
	return false;
}

void CShortestStrategy::__onPreDoStep()
{
	//改变导航点
	
}

void CShortestStrategy::__onPostDoStep()
{
}