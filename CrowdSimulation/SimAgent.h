#pragma once
#include "BaseAgent.h"

class CSimNode;

class CSimAgent : public IAgent
{
public:
	~CSimAgent();

	void setNavNode(CSimNode* pNavNode) { m_NavNode = pNavNode; }
	const CSimNode* getNavNode() const { return m_NavNode; }

private:
	CSimNode* m_NavNode = nullptr;
};

