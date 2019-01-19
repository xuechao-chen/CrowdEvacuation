#include "WeightedGraph.h"
#include <queue>
#include <set>

void CWeightedGraph::addNode(int vNodeIdx)
{
	_ASSERTE(m_NodeSet.find(vNodeIdx) == m_NodeSet.end());

	m_NodeSet[vNodeIdx] = AdjNodeSet();
}

void CWeightedGraph::addEdge(int vNodeIdx1, int vNodeIdx2, double vWeight)
{
	_ASSERTE(m_NodeSet.find(vNodeIdx1) != m_NodeSet.end() && m_NodeSet.find(vNodeIdx2) != m_NodeSet.end());

	for (auto it = m_NodeSet.begin(); it != m_NodeSet.end(); ++it)
	{
		if (it->first == vNodeIdx1)
		{
			it->second.push_back(std::make_pair(vNodeIdx2, vWeight));
		}
		if (it->first == vNodeIdx2)
		{
			it->second.push_back(std::make_pair(vNodeIdx1, vWeight));
		}
	}
}

void CWeightedGraph::removeNode(int vNodeIdx)
{
	_ASSERTE(m_NodeSet.find(vNodeIdx) == m_NodeSet.end());

	const AdjNodeSet& NodeSet = dumpAdjNodeSet(vNodeIdx);
	for (auto& AdjNode : NodeSet)
	{
		removeEdge(vNodeIdx, AdjNode.first);
	}

	for (auto it = m_NodeSet.begin(); it != m_NodeSet.end(); ++it)
	{
		if ((*it).first == vNodeIdx)
		{
			m_NodeSet.erase(it);
			break;
		}
	}
}

void CWeightedGraph::removeEdge(int vNodeIdx1, int vNodeIdx2)
{
	_ASSERTE(m_NodeSet.find(vNodeIdx1) != m_NodeSet.end() && m_NodeSet.find(vNodeIdx2) != m_NodeSet.end());

	__removeAdjNode(vNodeIdx1, vNodeIdx2);
	__removeAdjNode(vNodeIdx2, vNodeIdx1);
}

void CWeightedGraph::initGraph(int vNumOfNode)
{
	m_NodeSet.clear();

	for (auto i = 0; i < vNumOfNode; i++)
	{
		m_NodeSet[i] = AdjNodeSet();
	}
}

int CWeightedGraph::getNumEdges() const
{
	auto Num = 0;
	for (auto& Node : m_NodeSet)
	{
		Num += Node.second.size();
	}
	_ASSERTE(Num % 2 == 0);
	return Num / 2;
}

int CWeightedGraph::getNumAdjNodes(int vNodeIdx) const
{
	_ASSERTE(m_NodeSet.find(vNodeIdx) != m_NodeSet.end());

	for (auto it = m_NodeSet.begin(); it != m_NodeSet.end(); ++it)
	{
		if (it->first == vNodeIdx)
			return it->second.size();
	}
	return -1;
}

double CWeightedGraph::getEdgeWeight(int vFromNodeIdx, int vToNodeIdx) const
{
	_ASSERTE(m_NodeSet.find(vFromNodeIdx) != m_NodeSet.end() && m_NodeSet.find(vToNodeIdx) != m_NodeSet.end());

	for (auto it = m_NodeSet.begin(); it != m_NodeSet.end(); ++it)
	{
		if (it->first == vFromNodeIdx)
		{
			for (auto& AdjNode : it->second)
			{
				if (AdjNode.first == vToNodeIdx)
				{
					return AdjNode.second;
				}
			}
		}
	}

	return -1;
}

AdjNodeSet CWeightedGraph::dumpAdjNodeSet(int vFromNodeIdx) const
{
	_ASSERTE(m_NodeSet.find(vSourceNodeIdx) != m_NodeSet.end());

	for (auto it = m_NodeSet.begin(); it != m_NodeSet.end(); ++it)
	{
		if (it->first == vFromNodeIdx)
		{
			return it->second;
		}
	}

	return AdjNodeSet();
}

std::vector<int> CWeightedGraph::dumpAllNodeIdx() const
{
	auto NodeIdxSet = std::vector<int>();
	for (auto& Node : m_NodeSet)
	{
		NodeIdxSet.push_back(Node.first);
	}
	return NodeIdxSet;
}

void CWeightedGraph::updateEdgeWeight(int vFromNodeIdx, int vToNodeIdx, double vWeight)
{
	_ASSERTE(m_NodeSet.find(vFromNodeIdx) != m_NodeSet.end() && m_NodeSet.find(vToNodeIdx) != m_NodeSet.end());

	for (auto it = m_NodeSet.begin(); it != m_NodeSet.end(); ++it)
	{
		if (it->first == vFromNodeIdx || it->first == vToNodeIdx)
		{
			for (auto& AdjNode : it->second)
			{
				if (AdjNode.first == vToNodeIdx || AdjNode.first == vFromNodeIdx)
				{
					AdjNode.second = vWeight;
				}
			}
		}
	}

	return;
}

std::stack<int> CWeightedGraph::findShortestPath(int vFromNodeIdx, int vToNodeIdx) const
{
	auto Path = std::stack<int>();
	std::set<int> Q;
	auto PreviousMap = std::unordered_map<int, int>();
	auto DistMap = std::unordered_map<int,  double>();

	const auto& NodeIdxSet = dumpAllNodeIdx();
	for (auto NodeIdx : NodeIdxSet)
	{
		DistMap[NodeIdx] = FLT_MAX;
		PreviousMap[NodeIdx] = -1;
		Q.insert(NodeIdx);
	}

	DistMap[vFromNodeIdx] = 0;

	while (!Q.empty())
	{
		auto Min = FLT_MAX;
		auto u = -1;
		for (auto it = Q.begin(); it != Q.end(); ++it)
		{
			if (DistMap[*it] < Min)
			{
				Min = *it;
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

		if (PreviousMap[vToNodeIdx] != -1 || vToNodeIdx == vFromNodeIdx)
		{
			while (vToNodeIdx != -1)
			{
				Path.push(vToNodeIdx);
				vToNodeIdx = PreviousMap[vToNodeIdx];
			}
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
	
	return Path;
}

void CWeightedGraph::show()
{
	std::cout << "Graph size: " << m_NodeSet.size() << std::endl;
	for (auto& Node : m_NodeSet)
	{
		std::cout << Node.first << " : ";
		AdjNodeSet& NodeSet = Node.second;
		for (auto& AdjNode : NodeSet)
		{
			std::cout << "(" << AdjNode.first << ", " << AdjNode.second << ")";
		}
		std::cout << std::endl;
	}
}

void CWeightedGraph::__removeAdjNode(int vFromNodeIdx, int vAdjNodeIdx)
{
	_ASSERTE(m_NodeSet.find(vFromNodeIdx) != m_NodeSet.end() && m_NodeSet.find(vAdjNodeIdx) != m_NodeSet.end());

	AdjNodeSet& NodeSet = m_NodeSet[vFromNodeIdx];
	for (auto it = NodeSet.begin(); it != NodeSet.end(); ++it)
	{
		if ((*it).first == vAdjNodeIdx)
		{
			NodeSet.erase(it);
			break;
		}
	}
}
