#pragma once
#include <vector>
#include "GLM/glm.hpp"
#include "SimAgent.h"

enum class ESimNodeType 
{ 
	DivideNode, DistributionNode, NormalNode 
};

class SimNode
{
public:
	SimNode() {}
	SimNode(const glm::vec2& vPos) : m_Pos(vPos) {}
	virtual ~SimNode() {}

	void setNodeType(ESimNodeType vType) { m_Type = vType; }
	void setPos(const glm::vec2& vPos) { m_Pos = vPos; }
	void addSuccessor(const SimNode* vSuccessor) { m_SuccessorSet.push_back(vSuccessor); }

	ESimNodeType getNodeType() const { return m_Type; }
	glm::vec2 getPos() const { return m_Pos; }
	std::vector<const SimNode*> getSuccessorSet() const { return m_SuccessorSet; }

private:
	glm::vec2 m_Pos;
	std::vector<const SimNode*> m_SuccessorSet;
	ESimNodeType m_Type;
};