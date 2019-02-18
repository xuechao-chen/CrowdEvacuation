#pragma once
#include <vector>
#include "GLM/glm.hpp"
#include "BaseAgent.h"

enum class ESimNodeType 
{ 
	DivideNode, DistributionNode, NormalNode 
};

class CSimNode
{
public:
	CSimNode() {}
	CSimNode(const glm::vec2& vPos) : m_Pos(vPos) {}
	CSimNode(const glm::vec2& vPos, ESimNodeType vType) : m_Pos(vPos), m_Type(vType) {}
	virtual ~CSimNode() {}

	void setNodeType(ESimNodeType vType) { m_Type = vType; }
	void setPos(const glm::vec2& vPos) { m_Pos = vPos; }
	void addNavNode(const glm::vec2& vNavNode) { m_NavNodeSet.push_back(vNavNode);}

	ESimNodeType getNodeType() const { return m_Type; }
	glm::vec2 getPos() const { return m_Pos; }
	std::vector<glm::vec2> getNavNodeSet() const { return m_NavNodeSet; }
	glm::vec2 getNavNodeAt(int vIdx) const { return m_NavNodeSet[vIdx]; }
	int getNavNodeNum() const { return m_NavNodeSet.size(); }
	float getDistributionRatioAt(int vIdx) const { return m_DistributionRatioSet[vIdx]; }
	void setDistributionRatioAt(int vIdx, float vRatio) { m_DistributionRatioSet[vIdx] = vRatio; }
	void addDistributionRatio(float vRatio) { m_DistributionRatioSet.push_back(vRatio); }
	float getDivideRatio() const { return m_DivideRatio; }
	void setDivideRatio(float vRatio) { m_DivideRatio = vRatio; }
	void updateDividePos()
	{
		//TODO 根据导航点和分割率更新位置
	}

	bool isConverged();
	
private:
	glm::vec2 m_Pos;
	ESimNodeType m_Type;
	float m_DivideRatio = 0.5f;
	std::vector<float> m_DistributionRatioSet;
	std::vector<glm::vec2> m_NavNodeSet;
	std::vector<std::vector<std::vector<IAgent*>>> m_AgentGroups;
};