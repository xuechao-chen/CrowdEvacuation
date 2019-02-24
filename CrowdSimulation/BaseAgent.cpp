#include "BaseAgent.h"
#include "common/HiveCommonMicro.h"
#include "SceneGraph.h"

IAgent::~IAgent()
{
	_SAFE_DELETE(m_pRVOAgent);
}

void IAgent::setPosition(const glm::vec2 & vPos)
{
	m_pRVOAgent->setPosition(RVO::Vector2(vPos.x, vPos.y));
}

glm::vec2 IAgent::getPosition() const
{
	return {m_pRVOAgent->getPosition().x(), m_pRVOAgent->getPosition().y()};
}

void IAgent::setPrefVelocity(const glm::vec2 & vPrefVelocity)
{
	m_pRVOAgent->setPrefVelocity(RVO::Vector2(vPrefVelocity.x, vPrefVelocity.y));
}

bool IAgent::isReachNavNode() const
{
	const auto& Pos = getPosition();
	const auto& NavNode = getNavNode();
	return (abs(Pos.x - NavNode.x) < CSceneGraph::ROAD_WIDTH / 2 &&
		    abs(Pos.y - NavNode.y) < CSceneGraph::ROAD_WIDTH / 2);
}

bool IAgent::isReachExit(const std::vector<glm::vec2>& vExits) const
{
	if (m_IsReachExit) return true;

	for (auto& Exit : vExits)
	{
		auto Pos = getPosition();
		auto Distance = glm::distance(Pos, Exit);
		if (Distance < sqrt(1800)) return true;
	}

	return false;
}

void IAgent::reset()
{
	m_EvacuationTime = 0;
	m_IsReachExit = false;
	m_pRVOAgent->setPosition(RVO::Vector2(m_InitPos.x, m_InitPos.y));
}
