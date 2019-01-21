#pragma once
#include "SimNode.h"

class CDivideNode : public SimNode
{
public:
	CDivideNode() {}
	CDivideNode(const glm::vec2& vPos) : SimNode(vPos) { setNodeType(ESimNodeType::DivideNode); }

	bool isConverged() const;

private:
	std::vector<std::vector<CSimAgent*>> m_DivideAgentSet;
};

bool CDivideNode::isConverged() const
{
	return false;
}