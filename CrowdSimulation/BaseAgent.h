#pragma once
#include "../RVO/Agent.h"
#include "GLM/glm.hpp"

class IAgent
{
public:
	IAgent();
	virtual ~IAgent();

	void setLocatedEdge(const glm::vec2& vNode1, const glm::vec2& vNode2) { m_LocatedEdge.first = vNode1; m_LocatedEdge.second = vNode2; }
	std::pair<glm::vec2, glm::vec2> getLocatedEdge() const { return m_LocatedEdge; }

private:
	std::pair<glm::vec2, glm::vec2> m_LocatedEdge;
	RVO::Agent* m_pRVOAgent = nullptr;
};