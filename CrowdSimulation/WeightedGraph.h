#pragma once
#include <vector>
#include <unordered_map> 
#include <stack>
#include <iostream>
#include "common/HiveCommonMicro.h"

using NodeIdx = int;
using EdgeWeight = double;
using AdjNodeSet = std::vector<std::pair<NodeIdx, EdgeWeight>>;

// 通用的无向带权图，基于该图实现的最短路径算法
class CWeightedGraph
{
public:
	CWeightedGraph() {}
	CWeightedGraph(int vNumOfNode) { initGraph(vNumOfNode); }
	~CWeightedGraph() {}

	void initGraph(int vNumOfNode);
	void addNode(int vNodeIdx);
	void addEdge(int vNodeIdx1, int vNodeIdx2, double vWeight);
	void removeNode(int vNodeIdx);
	void removeEdge(int vNodeIdx1, int vNodeIdx2);
	int getNumNodes() const { return m_NodeSet.size(); }
	int getNumEdges() const;
	int getNumAdjNodes(int vNodeIdx) const;
	double getEdgeWeight(int vFromNodeIdx, int vToNodeIdx) const;
	AdjNodeSet dumpAdjNodeSet(int vFromNodeIdx) const;
    std::vector<int> dumpAllNodeIdx() const;
	void updateEdgeWeight(int vFromNodeIdx, int vToNodeIdx, double vWeight);
	std::stack<int> findShortestPath(int vFromNodeIdx, int vToNodeIdx) const;
	void show();

private:
	void __removeAdjNode(int vFromNodeIdx, int vAdjNodeIdx);

private:
	std::unordered_map<NodeIdx, AdjNodeSet> m_NodeSet;
};


