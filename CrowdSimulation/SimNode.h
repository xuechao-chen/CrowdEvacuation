#pragma once
#include <vector>
#include "GLM/glm.hpp"
#include "SimAgent.h"

enum class ESimNodeType 
{ 
	DivideNode, DistributionNode, NormalNode 
};

class CSimNode
{
public:
	CSimNode() {}
	CSimNode(const glm::vec2& vPos) : m_Pos(vPos) {}
	virtual ~CSimNode() {}

	void setNodeType(ESimNodeType vType) { m_Type = vType; }
	void setPos(const glm::vec2& vPos) { m_Pos = vPos; }
	void addNavNode(const glm::vec2& vNavNode) { m_NavNodeSet.push_back(vNavNode); }

	ESimNodeType getNodeType() const { return m_Type; }
	glm::vec2 getPos() const { return m_Pos; }
	std::vector<glm::vec2> getNavNodeSet() const { return m_NavNodeSet; }

private:
	glm::vec2 m_Pos;
	std::vector<glm::vec2> m_NavNodeSet;
	ESimNodeType m_Type;
};