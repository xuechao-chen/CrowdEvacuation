#pragma once
#include "../RVO/Agent.h"
#include "GLM/glm.hpp"

class IAgent
{
public:
	IAgent() = delete;
	IAgent(RVO::Agent* pRVOAgent) : m_pRVOAgent(pRVOAgent) {}
	IAgent(RVO::Agent* pRVOAgent, const glm::vec2& vPos) : m_pRVOAgent(pRVOAgent) { setPosition(vPos); };
	virtual ~IAgent();
	
	void setPosition(const glm::vec2& vPos); 
	glm::vec2 getPosition() const;
	void setPrefVelocity(const glm::vec2& vPrefVelocity);

private:
	RVO::Agent* m_pRVOAgent = nullptr;
};