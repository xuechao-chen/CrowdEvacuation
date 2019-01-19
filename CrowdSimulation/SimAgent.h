#pragma once
#include "BaseAgent.h"

class CSimAgent : public IAgent
{
public:
	CSimAgent();
	~CSimAgent();

private:
	int m_PathIdx;
};

