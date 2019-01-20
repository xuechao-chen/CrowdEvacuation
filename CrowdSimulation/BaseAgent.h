#pragma once
#include "../RVO/Agent.h"
#include "GLM/glm.hpp"

class IAgent
{
public:
	IAgent();
	virtual ~IAgent();

private:
	int m_Index;
	float m_Speed;
	bool m_RealGoal;
	int m_EscapeTime;
	int m_RoadmapIndex;
	glm::vec2  m_InitPosition;
	std::vector<glm::vec2> m_Roadmap;

	RVO::Agent* m_pRVOAgent = nullptr;
};

