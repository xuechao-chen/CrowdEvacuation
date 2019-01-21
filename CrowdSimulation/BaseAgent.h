#pragma once
#include "../RVO/Agent.h"
#include "GLM/glm.hpp"

class IAgent
{
public:
	IAgent();
	virtual ~IAgent();

private:
	RVO::Agent* m_pRVOAgent = nullptr;
};