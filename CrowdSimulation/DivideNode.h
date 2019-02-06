#pragma once
#include "SimNode.h"

class CDivideNode : public CSimNode
{
public:
	CDivideNode() {}
	CDivideNode(const glm::vec2& vPos) : CSimNode(vPos) { setNodeType(ESimNodeType::DivideNode); }

	bool isConverged() const;

private:
	std::vector<std::vector<CSimAgent*>> m_DivideAgentSet;
};

bool CDivideNode::isConverged() const
{
	return false;
}