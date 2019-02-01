#include "BaseAgent.h"
#include "common/HiveCommonMicro.h"

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
