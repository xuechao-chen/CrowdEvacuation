#pragma once
#include <unordered_map>
#include "common/HiveCommonMicro.h"
#include "GLM/glm.hpp"


using Node = glm::vec2;
using EdgeWeight = double;
using AdjNodePosSet = std::vector<std::pair<Node, EdgeWeight>>;
class HashFunc4Vec2
{
public:
	size_t operator()(glm::vec2 vNode) const
	{
		//use *cantor pairing function* for hash value
		//see [cantor pairing function](https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function)
		return (vNode.x + vNode.y)*(vNode.x + vNode.y + 1) / 2 + vNode.y;
	}
};

class CWeightedGraph
{
public:
	CWeightedGraph();
	CWeightedGraph(const std::string& vConfig);
	~CWeightedGraph();

	void constructGraph(const std::string& vConfig);
	void clearGraph();

	void addNode(const glm::vec2& vNode);
	void addEdge(const glm::vec2& vNode1, const glm::vec2& vNode2, double vWeight);
	void removeNode(const glm::vec2& vNode);
	void removeEdge(const glm::vec2& vNode1, const glm::vec2& vNode2);
	double getEdgeWeight(const glm::vec2& vNode1, const glm::vec2& vNode2) const;
	void updateEdgeWeight(const glm::vec2& vNode1, const glm::vec2& vNode2, double vWeight);

	std::vector<glm::vec2> dumpAllNodes() const;
	AdjNodePosSet dumpAdjNodeSet(const glm::vec2& vFromNode) const;
	std::vector<glm::vec2> findShortestPath(const glm::vec2& vFromNode, const glm::vec2& vToNode) const;

	int getNumNodes() const { return m_NodePosMap.size(); }
	int getNumEdges() const;

private:
	void __removeAdjNode(const glm::vec2& vAdjFromNode, const glm::vec2& vAdjToNode);

private:
	std::unordered_map<Node, AdjNodePosSet, HashFunc4Vec2> m_NodePosMap;
};