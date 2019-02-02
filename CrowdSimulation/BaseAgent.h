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

	void setNavNode(const glm::vec2& vNavNode) { m_NavNode = vNavNode; }
	glm::vec2 getNavNode() const { return m_NavNode; }
	bool isReachNavNode() const;
	bool isReachExit(const std::vector<glm::vec2>& vExits) const;
	
private:
	mutable bool m_IsReachExit = false;

	RVO::Agent* m_pRVOAgent = nullptr;
	glm::vec2 m_NavNode;
};