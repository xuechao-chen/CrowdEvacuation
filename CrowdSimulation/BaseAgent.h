#pragma once
#include "../RVO/Agent.h"
#include "GLM/glm.hpp"

class IAgent
{
public:
	IAgent();
	virtual ~IAgent();

	void setInitNavPoints(const std::vector<glm::vec2>& vNavPoints) { m_InitNavPoints = vNavPoints; }
	const std::vector<glm::vec2> getInitNavPoints() const { return m_InitNavPoints; }


private:
	RVO::Agent* m_pRVOAgent = nullptr;
	std::vector<glm::vec2> m_InitNavPoints;
};