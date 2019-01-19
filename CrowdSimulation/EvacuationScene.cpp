#include "EvacuationScene.h"
#include "common/HiveCommonMicro.h"

CEvacuationScene::CEvacuationScene()
{
}

CEvacuationScene::~CEvacuationScene()
{

}

void CEvacuationScene::addObstacle(const glm::vec2 & vLeftTop, const glm::vec2 & vRightBottom)
{
}

void CEvacuationScene::removeObstacle(const glm::vec2 & vLeftTop, const glm::vec2 & vRightBottom)
{
}

void CEvacuationScene::addNode(const glm::vec2 & vPos)
{
}

void CEvacuationScene::addEdge(const glm::vec2 & vPos1, const glm::vec2 & vPos2)
{
}

void CEvacuationScene::removeNode(const glm::vec2 & vPos)
{
}

void CEvacuationScene::removeEdge(const glm::vec2 & vPos1, const glm::vec2 & vPos2)
{
}

double CEvacuationScene::getEdgeWeight(const glm::vec2 & vPos1, const glm::vec2 & vPos2) const
{
	return 0.0;
}

void CEvacuationScene::updateEdgeWeight(const glm::vec2 & vPos1, const glm::vec2 & vPos2)
{
}

std::stack<glm::vec2> CEvacuationScene::findShortestPath(const glm::vec2 & vFromPos, const glm::vec2 & vToPos) const
{
	return std::stack<glm::vec2>();
}
