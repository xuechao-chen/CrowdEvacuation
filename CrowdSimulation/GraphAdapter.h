#pragma once
#include "WeightedGraph.h"
#include <unordered_map>
#include "GLM/glm.hpp"
#include <stack>

class CGraphAdapter : public CWeightedGraph
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
	std::stack<glm::vec2> findShortestPath(const glm::vec2& vFromPos, const glm::vec2& vToPos) const;

	int getIdx4Pos() const;
	glm::vec2 getPos4Idx() const;

private:
	std::unordered_map<int, glm::vec2> m_Idx2PosMap;
	std::unordered_map<glm::vec2, int> m_Pos2IdxMap;
};

