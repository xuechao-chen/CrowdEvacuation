#pragma once
#include "SimNode.h"

class CDistributionNode : public SimNode {

public:
	CDistributionNode() {}
	CDistributionNode(const glm::vec2& vPos) : SimNode(vPos) { setNodeType(ESimNodeType::DistributionNode); }

	void setDistributionRatio(const std::vector<double>& vDistributionRatioSet) { m_DistributionRatioSet = vDistributionRatioSet; }
	std::vector<double> getDistributionRatioSet() const { return m_DistributionRatioSet; }

	bool isConverged() const;

private:
	std::vector<double> m_DistributionRatioSet;
	std::vector<std::vector<CSimAgent*>> m_DistributionAgentSet;
};

bool CDistributionNode::isConverged() const
{
	return false;
}