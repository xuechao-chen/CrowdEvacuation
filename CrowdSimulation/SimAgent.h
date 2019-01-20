#pragma once
#include "BaseAgent.h"
#include "SimNode.h"

class CSimAgent : public IAgent
{
public:
	CSimAgent();
	~CSimAgent();

	void setNavNode(const SimNode* pNavNode) { m_NavNode = pNavNode; }
	const SimNode* getNavNode() const { return m_NavNode; }

private:
	const SimNode* m_NavNode = nullptr;
};

