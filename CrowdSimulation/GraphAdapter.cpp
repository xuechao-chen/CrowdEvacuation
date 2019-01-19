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

std::stack<glm::vec2> CGraphAdapter::findShortestPath(const glm::vec2 & vFromPos, const glm::vec2 & vToPos) const
{
	return std::stack<glm::vec2>();
}

int CGraphAdapter::getIdx4Pos() const
{
	return 0;
}

glm::vec2 CGraphAdapter::getPos4Idx() const
{
	return glm::vec2();
}
