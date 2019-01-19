#pragma once
#include "WeightedGraph.h"
#include <unordered_map>
#include "GLM/glm.hpp"

class HashFunc4Vec2
{
public:
	size_t operator()(const glm::vec2& vPos) const 
	{
		//use *cantor pairing function* for hash value
		//see [cantor pairing function](https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function)
		return (vPos.x + vPos.y)*(vPos.x + vPos.y + 1)/2 + vPos.y;
	}
};

class CGraphAdapter : protected CWeightedGraph
{
public:
	CGraphAdapter();
	CGraphAdapter(const std::string& vConfig);
	~CGraphAdapter();

	void constructGraph(const std::string& vConfig);
	void addNode(const glm::vec2& vPos);
	void addEdge(const glm::vec2& vPos1, const glm::vec2& vPos2);
	void removeNode(const glm::vec2& vPos);
	void removeEdge(const glm::vec2& vPos1, const glm::vec2& vPos2);
	double getEdgeWeight(const glm::vec2& vPos1, const glm::vec2& vPos2) const;
	void updateEdgeWeight(const glm::vec2& vPos1, const glm::vec2& vPos2);

	int getNumNodes() const { return CWeightedGraph::getNumNodes(); }
	int getNumEdges() const { return CWeightedGraph::getNumEdges(); }
	int getNumAdjNodes(const glm::vec2& vPos) const;
	std::vector<std::pair<glm::vec2, EdgeWeight>> dumpAdjNodeSet(const glm::vec2& vFromPos) const;
	std::vector<glm::vec2> dumpAllNodePos() const;
	std::vector<glm::vec2> findShortestPath(const glm::vec2& vFromPos, const glm::vec2& vToPos) const;

	int getIdx4Pos(const glm::vec2& vPos) const;
	glm::vec2 getPos4Idx(int vIdx) const;

private:
	std::unordered_map<int, glm::vec2> m_Idx2PosMap;
	std::unordered_map<glm::vec2, int, HashFunc4Vec2> m_Pos2IdxMap;
};

