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
	m_NodeMap.clear();
}

void CSceneGraph::addNode(const glm::vec2& vNode)
{
	_ASSERTE(m_NodeMap.find(vNode) == m_NodeMap.end());
	
	m_NodeMap[vNode] = AdjNodeSet();
}

void CSceneGraph::addEdge(const glm::vec2& vNode1, const glm::vec2& vNode2, double vWeight)
{
	_ASSERTE(m_NodeMap.find(vNode1) != m_NodeMap.end());
	_ASSERTE(m_NodeMap.find(vNode2) != m_NodeMap.end());
	_ASSERTE(vWeight >= 0);

	for (auto& it : m_NodeMap[vNode1])
	{
		if (it.first == vNode2) _ASSERTE(false);
	}

	for (auto it = m_NodeMap.begin(); it != m_NodeMap.end(); ++it)
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

	if (vNode1.x < vNode2.x || vNode1.y < vNode2.y)
	{
		m_EdgeSet.push_back(std::make_pair(vNode1, vNode2));
	}
	else
	{
		m_EdgeSet.push_back(std::make_pair(vNode2, vNode1));
	}
}

void CSceneGraph::removeNode(const glm::vec2& vNode)
{
	_ASSERTE(m_NodeMap.find(vNode) != m_NodeMap.end());
	
	for (auto& it : m_NodeMap)
	{
		if (it.first != vNode)
		{
			removeEdge(vNode, it.first);
		}
	}
	for (auto it = m_NodeMap.begin(); it != m_NodeMap.end(); ++it)
	{
		if (it->first == vNode)
		{
			m_NodeMap.erase(it);
			break;
		}
	}
}

void CSceneGraph::removeEdge(const glm::vec2& vNode1, const glm::vec2& vNode2)
{
	_ASSERTE(m_NodeMap.find(vNode1) != m_NodeMap.end());
	_ASSERTE(m_NodeMap.find(vNode2) != m_NodeMap.end());

	for (auto it = m_NodeMap[vNode1].begin(); it != m_NodeMap[vNode1].end(); ++it)
	{
		if (it->first == vNode2)
		{
			__removeAdjNode(vNode1, vNode2);
			__removeAdjNode(vNode2, vNode1);
			break;
		}
	}

	for (auto it = m_EdgeSet.begin(); it != m_EdgeSet.end(); ++it) 
	{
		if ((vNode1 == it->first && vNode2 == it->second) ||
			vNode1 == it->second && vNode2 == it->first)
		{
			m_EdgeSet.erase(it);
			break;
		}
	}
}

double CSceneGraph::getEdgeWeight(const glm::vec2& vNode1, const glm::vec2& vNode2) const
{
	_ASSERTE(m_NodeMap.find(vNode1) != m_NodeMap.end());
	_ASSERTE(m_NodeMap.find(vNode2) != m_NodeMap.end());

	for (const auto& itMap : m_NodeMap)
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
	_ASSERTE(m_NodeMap.find(vFromNode) != m_NodeMap.end());

	for (auto it = m_NodeMap.begin(); it != m_NodeMap.end(); ++it)
	{
		if (it->first == vFromNode)
		{
			return it->second;
		}
	}

	return AdjNodeSet();
}

std::pair<std::vector<glm::vec2>, float> CSceneGraph::findShortestPath(const glm::vec2& vFromNode, const glm::vec2& vToNode) const
{
	_ASSERTE(m_NodeMap.find(vFromNode) != m_NodeMap.end());
	_ASSERTE(m_NodeMap.find(vToNode) != m_NodeMap.end());

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
			return std::make_pair(Path, 0.0f);
			//TODO ¼ÆËãÂ·¾¶µÄ¾àÀë
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
	return std::make_pair(Path, FLT_MAX);
}

void CSceneGraph::updateEdgeWeight(const glm::vec2& vNode1, const glm::vec2& vNode2, double vWeight)
{
	_ASSERTE(m_NodeMap.find(vNode1) != m_NodeMap.end());
	_ASSERTE(m_NodeMap.find(vNode2) != m_NodeMap.end());
	_ASSERTE(vWeight >= 0);

	for (auto it = m_NodeMap.begin(); it != m_NodeMap.end(); ++it)
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
	for (auto& Node : m_NodeMap)
	{
		Num += Node.second.size();
	}
	Num /= 2;
	return Num;
}

std::vector<glm::vec2> CSceneGraph::dumpNavNodes(const glm::vec2 & vLocation) const
{
	std::vector<glm::vec2> NavNodes;
	auto HalfRoadWidth = ROAD_WIDTH / 2;

	for (auto& Edge : m_EdgeSet)
	{
		glm::vec2 LeftTopOfEdge     = { Edge.first.x - HalfRoadWidth, Edge.first.y - HalfRoadWidth };
		glm::vec2 RightBottomOfEdge = { Edge.second.x + HalfRoadWidth, Edge.second.y + HalfRoadWidth };
		if (__isLocationInRegion(vLocation, std::make_pair(LeftTopOfEdge, RightBottomOfEdge)))
		{
			auto& Node1 = Edge.first; auto& Node2 = Edge.second;
			glm::vec2 LeftTopOfNode1 = { Node1.x - HalfRoadWidth, Node1.y - HalfRoadWidth };
			glm::vec2 RightBottomOfNode1 = { Node1.x + HalfRoadWidth, Node1.y + HalfRoadWidth };
			glm::vec2 LeftTopOfNode2 = { Node2.x - HalfRoadWidth, Node2.y - HalfRoadWidth };
			glm::vec2 RightBottomOfNode2 = { Node2.x + HalfRoadWidth, Node2.y + HalfRoadWidth };

			if (__isLocationInRegion(vLocation, std::make_pair(LeftTopOfNode1, RightBottomOfNode1)))
			{
				NavNodes.push_back(Node1);
				break;
			}
			else if (__isLocationInRegion(vLocation, std::make_pair(LeftTopOfNode2, RightBottomOfNode2)))
			{
				NavNodes.push_back(Node2);
				break;
			}
			else
			{
				NavNodes.push_back(Node1);
				NavNodes.push_back(Node2);
				break;
			}
		}
	}

	return NavNodes;
}

std::vector<glm::vec2> CSceneGraph::dumpAllNodes() const
{
	auto NodeSet = std::vector<glm::vec2>();
	for (auto& Node : m_NodeMap)
	{
		NodeSet.push_back(Node.first);
	}
	return NodeSet;
}

std::vector<std::pair<glm::vec2, glm::vec2>> CSceneGraph::dumpAllEdges() const
{
	return m_EdgeSet;
}

void CSceneGraph::__removeAdjNode(const glm::vec2& vAdjFromNode, const glm::vec2& vAdjToNode)
{
	_ASSERTE(m_NodeMap.find(vAdjFromNode) != m_NodeMap.end());
	_ASSERTE(m_NodeMap.find(vAdjToNode) != m_NodeMap.end());

	AdjNodeSet& NodeSet = m_NodeMap[vAdjFromNode];
	for (auto it = NodeSet.begin(); it != NodeSet.end(); ++it)
	{
		if (it->first == vAdjToNode)
		{
			NodeSet.erase(it);
			break;
		}
	}
}

bool CSceneGraph::__isLocationInRegion(const glm::vec2 & vLocation, const std::pair<glm::vec2, glm::vec2>& vRegion) const
{
	if ((vLocation.x > vRegion.first.x && vLocation.x < vRegion.second.x)&&
		(vLocation.y > vRegion.first.y && vLocation.y < vRegion.second.y))
	{
		return true;
	}
	return false;
}
