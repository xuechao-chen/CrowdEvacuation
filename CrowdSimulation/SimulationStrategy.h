#pragma once
#include "BaseStrategy.h"
#include "SimNode.h"

class CSimulationStrategy : public IEvacuationStrategy
{
public:
	CSimulationStrategy();
	~CSimulationStrategy();

private:
	bool __isFinish() override;
	void __onPreDoStep() override;
	void __onPostDoStep() override;
	void __constructRoadMap() override;
	void __assignNavNode2Agent() override;

	void __analyzeConvergence();
	void __addShortestPath2RoadMap(const std::vector<glm::vec2> & ShortestPath);
	bool __isAllAgentReachExit();
	void __updateAgentsVelocity();
	void __updateScene();
	void __updateDivideNode(CSimNode* pSimNode);
	void __updateDistributionNode(CSimNode* pSimNode);
	void __resetAgents();

	void __constructRoadMapFromFile();
	void __constructBasicRoadMap();
	void __addDivideNode2RoadMap();
	void __addDistributionNode2RoadMap();
	
private:
	int m_IterationNum = 0;
	bool m_IsConverged = false;
	std::unordered_map<glm::vec2, CSimNode*, HashFunc4Node> m_RoadMap;
	std::unordered_map<glm::vec2, CSimNode*, HashFunc4Node> m_BasicRoadMap;
};