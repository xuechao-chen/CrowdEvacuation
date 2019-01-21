#pragma once
#include "../RVO/Agent.h"
#include "GLM/glm.hpp"

class IAgent
{
public:
	IAgent();
	virtual ~IAgent();

	void test()
	{
		auto goal = m_pRVOAgent->getGoalPos();
		m_pRVOAgent->setGoalPos(goal);
	}

private:
	RVO::Agent* m_pRVOAgent = nullptr;
};