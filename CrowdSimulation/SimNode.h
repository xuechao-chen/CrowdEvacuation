#pragma once
#include <vector>
#include "GLM/glm.hpp"
#include "SimAgent.h"

enum class ESimNodeType 
{ 
	DivideNode, DistributionNode, NormalNode 
};

class ISimNode
{
public:
	ISimNode() {}
	ISimNode(const glm::vec2& vPos) : m_Pos(vPos) {}
	virtual ~ISimNode() {}

	void setNodeType(ESimNodeType vType) { m_Type = vType; }
	void setPos(const glm::vec2& vPos) { m_Pos = vPos; }
	void addSuccessor(const ISimNode* vSuccessor) { m_SuccessorSet.push_back(vSuccessor); }

	ESimNodeType getNodeType() const { return m_Type; }
	glm::vec2 getPos() const { return m_Pos; }
	std::vector<const ISimNode*> getSuccessorSet() const { return m_SuccessorSet; }

private:
	glm::vec2 m_Pos;
	std::vector<const ISimNode*> m_SuccessorSet;
	ESimNodeType m_Type;
};

class CDivideNode : public ISimNode
{
public:
	CDivideNode() {}
	CDivideNode(const glm::vec2& vPos) : ISimNode(vPos) { setNodeType(ESimNodeType::DivideNode); }

	bool isConverged() const;

private:
	std::vector<std::vector<CSimAgent*>> m_DivideAgentSet;
};

class CDistributionNode : public ISimNode {

public:
	CDistributionNode() {}
	CDistributionNode(const glm::vec2& vPos) : ISimNode(vPos) { setNodeType(ESimNodeType::DistributionNode); }

	void setDistributionRatio(const std::vector<double>& vDistributionRatioSet) { m_DistributionRatioSet = vDistributionRatioSet; }
	std::vector<double> getDistributionRatioSet() const { return m_DistributionRatioSet; }

	bool isConverged() const;

private:
	std::vector<double> m_DistributionRatioSet;
	std::vector<std::vector<CSimAgent*>> m_DistributionAgentSet;
};