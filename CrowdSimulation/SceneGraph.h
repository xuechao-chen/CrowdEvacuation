#pragma once
#include <unordered_map>
#include "common/HiveCommonMicro.h"
#include "GLM/glm.hpp"

using Node = glm::vec2;
using EdgeWeight = double;
using AdjNodeSet = std::vector<std::pair<Node, EdgeWeight>>;

class HashFunc4Node
{
public:
	size_t operator()(glm::vec2 vNode) const
	{
		//use *cantor pairing function* for hash value
		//see [cantor pairing function](https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function)
		return (vNode.x + vNode.y)*(vNode.x + vNode.y + 1) / 2 + vNode.y;
	}
};

class CSceneGraph
{
public:
	static const int ROAD_WIDTH = 60;

	CSceneGraph();
	CSceneGraph(const std::string& vConfig);
	~CSceneGraph();

	void clearGraph();

	void addNode(const glm::vec2& vNode);
	void removeNode(const glm::vec2& vNode);
	void addEdge(const glm::vec2& vNode1, const glm::vec2& vNode2, double vWeight);
	void removeEdge(const glm::vec2& vNode1, const glm::vec2& vNode2);
	double getEdgeWeight(const glm::vec2& vNode1, const glm::vec2& vNode2) const;
	void updateEdgeWeight(const glm::vec2& vNode1, const glm::vec2& vNode2, double vWeight);
	int getNumNodes() const { return m_NodeMap.size(); }
	int getNumEdges() const;

	std::vector<glm::vec2> dumpNavNodes(const glm::vec2& vLocation) const;
	std::vector<glm::vec2> dumpAllNodes() const;
	std::vector<std::pair<glm::vec2, glm::vec2>> dumpAllEdges() const;
	AdjNodeSet dumpAdjNodeSet(const glm::vec2& vFromNode) const;
	std::pair<std::vector<glm::vec2>, float> findShortestPath(const glm::vec2& vFromNode, const glm::vec2& vToNode) const;

private:
	void __removeAdjNode(const glm::vec2& vAdjFromNode, const glm::vec2& vAdjToNode);
	bool __isLocationInRegion(const glm::vec2& vLocation, const std::pair<glm::vec2, glm::vec2>& vRegion) const;

private:
	std::unordered_map<Node, AdjNodeSet, HashFunc4Node> m_NodeMap;
	std::vector<std::pair<glm::vec2, glm::vec2>> m_EdgeSet; 
	//为了查找方便，额外存储的数据结构,边的方向按左上到右下存储
};