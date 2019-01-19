#include "GraphAdapter.h"

CGraphAdapter::CGraphAdapter()
{
}

CGraphAdapter::CGraphAdapter(const std::string & vConfig)
{
	constructGraph(vConfig);
}

CGraphAdapter::~CGraphAdapter()
{
}

void CGraphAdapter::constructGraph(const std::string & vConfig)
{
}

void CGraphAdapter::addNode(const glm::vec2 & vPos)
{
}

void CGraphAdapter::addEdge(const glm::vec2 & vPos1, const glm::vec2 & vPos2)
{
}

void CGraphAdapter::removeNode(const glm::vec2 & vPos)
{
}

void CGraphAdapter::removeEdge(const glm::vec2 & vPos1, const glm::vec2 & vPos2)
{
}

double CGraphAdapter::getEdgeWeight(const glm::vec2 & vPos1, const glm::vec2 & vPos2) const
{
	return 0.0;
}

void CGraphAdapter::updateEdgeWeight(const glm::vec2 & vPos1, const glm::vec2 & vPos2)
{
}

std::vector<glm::vec2> CGraphAdapter::findShortestPath(const glm::vec2 & vFromPos, const glm::vec2 & vToPos) const
{
	return std::vector<glm::vec2>();
}

int CGraphAdapter::getNumAdjNodes(const glm::vec2 & vPos) const
{
	return 0;
}

std::vector<std::pair<glm::vec2, EdgeWeight>> CGraphAdapter::dumpAdjNodeSet(const glm::vec2 & vFromPos) const
{
	return std::vector<std::pair<glm::vec2, EdgeWeight>>();
}

std::vector<glm::vec2> CGraphAdapter::dumpAllNodePos() const
{
	return std::vector<glm::vec2>();
}

int CGraphAdapter::getIdx4Pos(const glm::vec2 & vPos) const
{
	return 0;
}

glm::vec2 CGraphAdapter::getPos4Idx(int vIdx) const
{
	return glm::vec2();
}
