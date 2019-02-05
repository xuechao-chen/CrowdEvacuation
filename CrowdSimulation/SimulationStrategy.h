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

	/*void __initEvacuationDirection();
	void __updateEvacuationDirection();
	void __assignAgentNavNode();
	bool __isConverged() const;
	void __doSimulation();*/
	void __analyzeConvergence();
	bool __isDivideNodeConverged();
	bool __isDistributionNodeConverged();
	
private:
	std::vector<SimNode*> m_NodeSet;
	int m_MaxIterationNum = 0;
	bool m_IsConverged = false;
};