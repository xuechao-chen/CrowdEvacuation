#pragma once
#include "BaseStrategy.h"
#include "SimNode.h"

class CSimulationStrategy : public IEvacuationStrategy
{
public:
	CSimulationStrategy() = default;
	~CSimulationStrategy() = default;

private:
	bool __isFinish() override;
	void __afterSimulationDoStep() override;
	void __constructRoadMap() override;
	void __assignNavNode2Agent() override;

	void __addShortestPath2RoadMap(const std::vector<glm::vec2>& ShortestPath);
	void __updateAgentsVelocity();
	void __updateScene();
	void __updateDivideNode(CSimNode* pSimNode);
	void __updateDistributionNode(CSimNode* pSimNode);
	void __resetAgents();
	
private:
	int m_IterationNum = 0;
	std::unordered_map<glm::vec2, CSimNode*, HashFunc4Node> m_RoadMap;
};