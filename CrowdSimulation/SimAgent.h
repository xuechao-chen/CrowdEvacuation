#pragma once
#include "BaseAgent.h"
#include "SimNode.h"

class CSimAgent : public IAgent
{
public:
	CSimAgent();
	~CSimAgent();

	void setNavNode(const ISimNode* pNavNode) { m_NavNode = pNavNode; }
	const ISimNode* getNavNode() const { return m_NavNode; }

private:
	const ISimNode* m_NavNode = nullptr;
};

