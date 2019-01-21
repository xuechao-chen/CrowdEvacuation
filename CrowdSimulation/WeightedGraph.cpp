#include <algorithm>
#include "WeightedGraph.h"
#include "ConfigParser.h"
#include <vector>

CWeightedGraph::CWeightedGraph()
{
}

CWeightedGraph::CWeightedGraph(const std::string & vConfig)
{
	constructGraph(vConfig);
}


CWeightedGraph::~CWeightedGraph()
{

}

void CWeightedGraph::constructGraph(const std::string & vConfig)
{
	clearGraph();
	CConfigParser::getInstance()->parseGraph(vConfig, m_NodePosMap);
}



void CWeightedGraph::clearGraph()
{
	m_NodePosMap.clear();
}

void CWeightedGraph::addNode(const glm::vec2& vPos)
{
	if (m_NodePosMap.find(vPos) == m_NodePosMap.end())
	{
		m_NodePosMap[vPos] = AdjNodePosSet();

	}
	else
	{
		std::cout << "the pos is already has a node" << std::endl;
	}
}

void CWeightedGraph::addEdge(const glm::vec2& vNodePos1, const glm::vec2& vNodePos2, double vWeight)
{
	if (m_NodePosMap.find(vNodePos1) == m_NodePosMap.end() || m_NodePosMap.find(vNodePos2) == m_NodePosMap.end() || vNodePos1 == vNodePos2 || vWeight < 0)
		return;
	for (auto& it : m_NodePosMap[vNodePos1])
	{
		if (it.first == vNodePos2)
			return;
	}

	for (auto it = m_NodePosMap.begin(); it != m_NodePosMap.end(); ++it)
	{
		if (it->first == vNodePos1)
		{
			it->second.push_back(std::make_pair(vNodePos2, vWeight));
		}
		if (it->first == vNodePos2)
		{
			it->second.push_back(std::make_pair(vNodePos1, vWeight));
		}
	}
}

void CWeightedGraph::removeNode(const glm::vec2& vNodePos)
{
	if (m_NodePosMap.find(vNodePos) == m_NodePosMap.end())
	{
		std::cout << "the pos is doesn't have a node" << std::endl;
	}
	else
	{
		for (auto& it : m_NodePosMap)
		{
			if (it.first != vNodePos)
			{
				removeEdge(vNodePos, it.first);
			}
		}
		for (auto it = m_NodePosMap.begin(); it != m_NodePosMap.end(); it++)
		{
			if (it->first == vNodePos)
			{
				m_NodePosMap.erase(it);
				break;
			}
		}
	}
}

void CWeightedGraph::removeEdge(const glm::vec2& vNodePos1, const glm::vec2& vNodePos2)
{
	if (m_NodePosMap.find(vNodePos1) == m_NodePosMap.end() || m_NodePosMap.find(vNodePos2) == m_NodePosMap.end() || vNodePos1 == vNodePos2)
		return;
	for (auto it = m_NodePosMap[vNodePos1].begin(); it != m_NodePosMap[vNodePos1].end(); it++)
	{
		if (it->first == vNodePos2)
		{
			__removeAdjNode(vNodePos1, vNodePos2);
			__removeAdjNode(vNodePos2, vNodePos1);
			break;
		}
	}
}

double CWeightedGraph::getEdgeWeight(const glm::vec2& vFromNodePos, const glm::vec2& vToNodePos) const
{

	for (const auto& itMap : m_NodePosMap)
	{
		if (itMap.first == vFromNodePos)
		{
			for (const auto& itVec : itMap.second)
			{
				if (itVec.first == vToNodePos)
				{
					return itVec.second;
				}
			}
		}
	}
	return 0.0;
}

AdjNodePosSet CWeightedGraph::dumpAdjNodeSet(const glm::vec2& vFromNodePos) const
{
	for (auto it = m_NodePosMap.begin(); it != m_NodePosMap.end(); ++it)
	{
		if (it->first == vFromNodePos)
		{
			return it->second;
		}
	}
	return AdjNodePosSet();
}

std::vector<glm::vec2> CWeightedGraph::findShortestPath(const glm::vec2& vFromNodePos, const glm::vec2& vToNodePos) const
{
	auto Path = std::vector<glm::vec2>();
	std::vector<glm::vec2> Q;
	auto PreviousMap = std::unordered_map<glm::vec2, glm::vec2, HashFunc4Vec2>();
	auto DistMap = std::unordered_map<glm::vec2, double, HashFunc4Vec2>();
	glm::vec2 toNodePos = vToNodePos;
	const auto& NodePosSet = dumpAllNodes();
	for (auto NodePos : NodePosSet)
	{
		DistMap[NodePos] = FLT_MAX;
		PreviousMap[NodePos] = glm::vec2(-1, -1);
		Q.push_back(NodePos);//set 不支持vec2类型
	}

	//std::cout << fromNodeIdx << std::endl;
	//std::cout << toNodeIdx << std::endl;

	DistMap[vFromNodePos] = 0;

	while (!Q.empty())
	{
		auto Min = FLT_MAX;
		auto u = glm::vec2(-1, -1);
		for (auto it = Q.begin(); it != Q.end(); ++it)
		{
			if (DistMap[*it] < Min)
			{
				Min = DistMap[*it];
				u = *it;
			}
		}

		for (auto it = Q.begin(); it != Q.end(); ++it)
		{
			if (*it == u)
			{
				Q.erase(it);
			}
		}

		if (PreviousMap[toNodePos] != glm::vec2(-1, -1) || toNodePos == vFromNodePos)
		{
			while (toNodePos != glm::vec2(-1, -1))
			{
				Path.push_back(toNodePos);
				toNodePos = PreviousMap[toNodePos];
			}
			std::reverse(Path.begin(), Path.end());
			return Path;
		}

		const auto& AdjNodeSet = dumpAdjNodeSet(u);
		for (auto& Node : AdjNodeSet)
		{
			auto v = Node.first;
			auto Distance = getEdgeWeight(u, v);
			auto Alt = DistMap[u] + Distance;
			if (DistMap[v] > Alt)
			{
				DistMap[v] = Alt;
				PreviousMap[v] = u;
			}
		}
	}
	std::reverse(Path.begin(), Path.end());
	return Path;
}

void CWeightedGraph::updateEdgeWeight(const glm::vec2& vFromNodePos, const glm::vec2& vToNodePos, double vWeight)
{
	if (m_NodePosMap.find(vFromNodePos) == m_NodePosMap.end() || m_NodePosMap.find(vToNodePos) == m_NodePosMap.end() || vFromNodePos == vToNodePos || vWeight < 0)
		return;
	for (auto it = m_NodePosMap.begin(); it != m_NodePosMap.end(); ++it)
	{
		if (it->first == vFromNodePos || it->first == vToNodePos)
		{
			for (auto& AdjNode : it->second)
			{
				if (AdjNode.first == vToNodePos || AdjNode.first == vFromNodePos)
				{
					AdjNode.second = vWeight;
					return;
				}
			}
		}
	}
}


int CWeightedGraph::getNumEdges() const
{
	int Num = 0;
	for (auto& Node : m_NodePosMap)
	{
		Num += Node.second.size();
	}
	Num /= 2;
	return Num;
}

std::vector<glm::vec2> CWeightedGraph::dumpAllNodes() const
{
	auto NodePosSet = std::vector<glm::vec2>();
	for (auto& Node : m_NodePosMap)
	{
		NodePosSet.push_back(Node.first);
	}
	return NodePosSet;
}

void CWeightedGraph::__removeAdjNode(const glm::vec2& vAdjFromNodePos, const glm::vec2& vAdjToNodePos)
{
	if (m_NodePosMap.find(vAdjFromNodePos) == m_NodePosMap.end())
		return;
	AdjNodePosSet& NodePosSet = m_NodePosMap[vAdjFromNodePos];
	if (NodePosSet.size() == 0)
		return;
	for (auto it = NodePosSet.begin(); it != NodePosSet.end(); ++it)
	{
		if (it->first == vAdjToNodePos)
		{
			NodePosSet.erase(it);
			break;
		}
	}
}