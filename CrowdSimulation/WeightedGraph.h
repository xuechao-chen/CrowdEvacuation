#pragma once
#include <vector>
#include <unordered_map> 
#include "common/HiveCommonMicro.h"
#include "GLM/glm.hpp"

using NodeIdx = int;
using EdgeWeight = double;
using AdjNodeSet = std::vector<std::pair<NodeIdx, EdgeWeight>>;

class HashFunc4Vec2
{
public:
	size_t operator()(const glm::vec2& vNode) const
	{
		//use *cantor pairing function* for hash value
		//see [cantor pairing function](https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function)
		return (vNode.x + vNode.y)*(vNode.x + vNode.y + 1) / 2 + vNode.y;
	}
};

// 通用的无向带权图，基于该图实现的最短路径算法
class CWeightedGraph
{
public:
	CWeightedGraph() {}
	~CWeightedGraph() {}

	void constructGraph(const std::string& vConfig);

	void addNode(const glm::vec2& vNode);
	void addEdge(const glm::vec2& vNode1, const glm::vec2& vNode2);
	void removeNode(const glm::vec2& vNode);
	void removeEdge(const glm::vec2& vNode1, const glm::vec2& vNode2);

	int getNumNodes() const;
	int getNumEdges() const;
	int getNumAdjNodes(const glm::vec2& vNode) const;
	double getEdgeWeight(const glm::vec2& vNode1, const glm::vec2& vNode2) const;
	void updateEdgeWeight(const glm::vec2& vNode1, const glm::vec2& vNode2);

	std::vector<std::pair<glm::vec2, EdgeWeight>> dumpAdjNodeSet(const glm::vec2& vFromNode) const;
	std::vector<glm::vec2> dumpAllNodeNode() const;
	std::vector<glm::vec2> findShortestPath(const glm::vec2& vFromNode, const glm::vec2& vToNode) const;

	int getIdx4Node(const glm::vec2& vNode) const;
	glm::vec2 getNode4Idx(int vIdx) const;

private:
	void __removeAdjNode(int vFromNode, int vAdjNode);

private:
	std::unordered_map<NodeIdx, AdjNodeSet> m_NodeSet;
	std::unordered_map<int, glm::vec2> m_Idx2NodeMap;
	std::unordered_map<glm::vec2, int, HashFunc4Vec2> m_Node2IdxMap;
};


