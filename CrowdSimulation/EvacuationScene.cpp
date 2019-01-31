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
	m_Obstacles.push_back(std::make_pair(vLeftTop, vRightBottom));
}

void CEvacuationScene::removeObstacle(const glm::vec2 & vLeftTop, const glm::vec2 & vRightBottom)
{
	for (auto it = m_Obstacles.begin(); it != m_Obstacles.end(); ++it)
	{
		if (vLeftTop == it->first && vRightBottom == it->second)
		{
			m_Obstacles.erase(it);
			break;
		}
	}
}

void CEvacuationScene::addExit(const glm::vec2 & vExit)
{
	m_Exits.push_back(vExit);
}

void CEvacuationScene::removeExit(const glm::vec2 & vExit)
{
	for (auto it = m_Exits.begin(); it != m_Exits.end(); ++it)
	{
		if (vExit == *it)
		{
			m_Exits.erase(it);
			break;
		}
	}
}