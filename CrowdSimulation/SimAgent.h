#pragma once
#include "BaseAgent.h"

class SimNode;

class CSimAgent : public IAgent
{
public:
	~CSimAgent();

	void setNavNode(SimNode* pNavNode) { m_NavNode = pNavNode; }
	const SimNode* getNavNode() const { return m_NavNode; }

private:
	SimNode* m_NavNode = nullptr;
};

