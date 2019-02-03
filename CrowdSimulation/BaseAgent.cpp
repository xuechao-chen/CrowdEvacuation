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
	auto Distance = glm::distance(getNavNode(), getPosition());
	if (Distance < CSceneGraph::ROAD_WIDTH/2) return true;
	else return false;
}

bool IAgent::isReachExit(const std::vector<glm::vec2>& vExits) const
{
	if (m_IsReachExit) return true;

	for (auto& Exit : vExits)
	{
		auto Pos = getPosition();
		auto Distance = glm::distance(Pos, Exit);
		if (Distance > 6) return false; //TODO: ÔÝ¶¨°ë¾¶Îª3
	}

	m_IsReachExit = true;
	return true;
}
