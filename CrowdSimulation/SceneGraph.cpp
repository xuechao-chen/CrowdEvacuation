#include <algorithm>
#include "SceneGraph.h"
#include "ConfigParser.h"
#include <vector>

CSceneGraph::CSceneGraph()
{
}

CSceneGraph::CSceneGraph(const std::string & vConfig)
{
}

CSceneGraph::~CSceneGraph()
{
}

void CSceneGraph::clearGraph()
{
	m_NodePosMap.clear();
}

void CSceneGraph::addNode(const glm::vec2& vNode)
{
	_ASSERTE(m_NodePosMap.find(vNode) == m_NodePosMap.end());
	
	m_NodePosMap[vNode] = AdjNodeSet();
}

void CSceneGraph::addEdge(const glm::vec2& vNode1, const glm::vec2& vNode2, double vWeight)
{
	_ASSERTE(m_NodePosMap.find(vNode1) != m_NodePosMap.end());
	_ASSERTE(m_NodePosMap.find(vNode2) != m_NodePosMap.end());
	_ASSERTE(vWeight >= 0);

	for (auto& it : m_NodePosMap[vNode1])
	{
		if (it.first == vNode2) _ASSERTE(false);
	}

	for (auto it = m_NodePosMap.begin(); it != m_NodePosMap.end(); ++it)
	{
		if (it->first == vNode1)
		{
			it->second.push_back(std::make_pair(vNode2, vWeight));
		}
		if (it->first == vNode2)
		{
			it->second.push_back(std::make_pair(vNode1, vWeight));
		}
	}
}

void CSceneGraph::removeNode(const glm::vec2& vNode)
{
	_ASSERTE(m_NodePosMap.find(vNode) != m_NodePosMap.end());
	
	for (auto& it : m_NodePosMap)
	{
		if (it.first != vNode)
		{
			removeEdge(vNode, it.first);
		}
	}
	for (auto it = m_NodePosMap.begin(); it != m_NodePosMap.end(); ++it)
	{
		if (it->first == vNode)
		{
			m_NodePosMap.erase(it);
			break;
		}
	}
}

void CSceneGraph::removeEdge(const glm::vec2& vNode1, const glm::vec2& vNode2)
{
	_ASSERTE(m_NodePosMap.find(vNode1) != m_NodePosMap.end());
	_ASSERTE(m_NodePosMap.find(vNode2) != m_NodePosMap.end());

	for (auto it = m_NodePosMap[vNode1].begin(); it != m_NodePosMap[vNode1].end(); it++)
	{
		if (it->first == vNode2)
		{
			__removeAdjNode(vNode1, vNode2);
			__removeAdjNode(vNode2, vNode1);
			break;
		}
	}
}

double CSceneGraph::getEdgeWeight(const glm::vec2& vNode1, const glm::vec2& vNode2) const
{
	_ASSERTE(m_NodePosMap.find(vNode1) != m_NodePosMap.end());
	_ASSERTE(m_NodePosMap.find(vNode2) != m_NodePosMap.end());

	for (const auto& itMap : m_NodePosMap)
	{
		if (itMap.first == vNode1)
		{
			for (const auto& itVec : itMap.second)
			{
				if (itVec.first == vNode2)
				{
					return itVec.second;
				}
			}
		}
	}

	return 0.0;
}

AdjNodeSet CSceneGraph::dumpAdjNodeSet(const glm::vec2& vFromNode) const
{
	_ASSERTE(m_NodePosMap.find(vFromNode) != m_NodePosMap.end());

	for (auto it = m_NodePosMap.begin(); it != m_NodePosMap.end(); ++it)
	{
		if (it->first == vFromNode)
		{
			return it->second;
		}
	}

	return AdjNodeSet();
}

std::vector<glm::vec2> CSceneGraph::findShortestPath(const glm::vec2& vFromNode, const glm::vec2& vToNode) const
{
	_ASSERTE(m_NodePosMap.find(vFromNode) != m_NodePosMap.end());
	_ASSERTE(m_NodePosMap.find(vToNode) != m_NodePosMap.end());

	auto Path = std::vector<glm::vec2>();
	std::vector<glm::vec2> Q;
	auto PreviousMap = std::unordered_map<glm::vec2, glm::vec2, HashFunc4Node>();
	auto DistMap = std::unordered_map<glm::vec2, double, HashFunc4Node>();
	glm::vec2 toNodePos = vToNode;
	const auto& NodePosSet = dumpAllNodes();
	for (auto NodePos : NodePosSet)
	{
		DistMap[NodePos] = FLT_MAX;
		PreviousMap[NodePos] = glm::vec2(-1, -1);
		Q.push_back(NodePos);
	}

	DistMap[vFromNode] = 0;

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
				break;
			}
		}

		if (PreviousMap[toNodePos] != glm::vec2(-1, -1) || toNodePos == vFromNode)
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

void CSceneGraph::updateEdgeWeight(const glm::vec2& vNode1, const glm::vec2& vNode2, double vWeight)
{
	_ASSERTE(m_NodePosMap.find(vNode1) != m_NodePosMap.end());
	_ASSERTE(m_NodePosMap.find(vNode2) != m_NodePosMap.end());
	_ASSERTE(vWeight >= 0);

	for (auto it = m_NodePosMap.begin(); it != m_NodePosMap.end(); ++it)
	{
		if (it->first == vNode1 || it->first == vNode2)
		{
			for (auto& AdjNode : it->second)
			{
				if (AdjNode.first == vNode2 || AdjNode.first == vNode1)
				{
					AdjNode.second = vWeight;
					break;
				}
			}
		}
	}
}

int CSceneGraph::getNumEdges() const
{
	int Num = 0;
	for (auto& Node : m_NodePosMap)
	{
		Num += Node.second.size();
	}
	Num /= 2;
	return Num;
}

std::vector<glm::vec2> CSceneGraph::dumpAllNodes() const
{
	auto NodeSet = std::vector<glm::vec2>();
	for (auto& Node : m_NodePosMap)
	{
		NodeSet.push_back(Node.first);
	}
	return NodeSet;
}

void CSceneGraph::__removeAdjNode(const glm::vec2& vAdjFromNode, const glm::vec2& vAdjToNode)
{
	_ASSERTE(m_NodePosMap.find(vAdjFromNode) != m_NodePosMap.end());
	_ASSERTE(m_NodePosMap.find(vAdjToNode) != m_NodePosMap.end());

	AdjNodeSet& NodeSet = m_NodePosMap[vAdjFromNode];
	for (auto it = NodeSet.begin(); it != NodeSet.end(); ++it)
	{
		if (it->first == vAdjToNode)
		{
			NodeSet.erase(it);
			break;
		}
	}
}