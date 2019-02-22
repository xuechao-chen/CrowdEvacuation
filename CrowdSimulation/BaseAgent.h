#pragma once
#include "../RVO/Agent.h"
#include "GLM/glm.hpp"

class IAgent
{
public:
	IAgent() = delete;
	IAgent(RVO::Agent* pRVOAgent) : m_pRVOAgent(pRVOAgent) {}
	IAgent(RVO::Agent* pRVOAgent, const glm::vec2& vPos) : m_pRVOAgent(pRVOAgent) { setPosition(vPos); m_InitPos = vPos; };
	virtual ~IAgent();
	
	void setPosition(const glm::vec2& vPos); 
	glm::vec2 getPosition() const;
	glm::vec2 getInitPosition() const { return m_InitPos; }
	void setPrefVelocity(const glm::vec2& vPrefVelocity);

	void setNavNode(const glm::vec2& vNavNode) { m_NavNode = vNavNode; }
	glm::vec2 getNavNode() const { return m_NavNode; }
	void setLastNavNode(const glm::vec2& vLastNavNode) { m_LastNavNode = vLastNavNode; }
	glm::vec2 getLastNavNode() const { return m_LastNavNode; }
	bool isReachNavNode() const;
	bool isReachExit(const std::vector<glm::vec2>& vExits) const;
	void tagIsReachExit(bool vIsReachExit) { m_IsReachExit = vIsReachExit; }

	void setEvacuationTime(int vTime) { if (!m_IsReachExit) m_EvacuationTime = vTime;}
	int getEvacuationTime() const { return m_EvacuationTime; }
	void reset();

	float getSpeed()
	{
		auto Velocity = m_pRVOAgent->getVelocity();
		return glm::distance(glm::vec2{ Velocity.x(), Velocity.y() }, { 0, 0 });
	}
	void setSpeed(float vSpeed)
	{
		m_pRVOAgent->setSpeed(vSpeed);
	}

private:
	mutable bool m_IsReachExit = false;

	RVO::Agent* m_pRVOAgent = nullptr;
	glm::vec2 m_NavNode;
	glm::vec2 m_LastNavNode;
	glm::vec2 m_InitPos;
	int m_EvacuationTime = 0;
};