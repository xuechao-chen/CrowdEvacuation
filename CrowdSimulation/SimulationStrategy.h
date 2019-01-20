#pragma once
#include "BaseEvacuationStrategy.h"
#include "SimNode.h"

class CSimulationStrategy : public IEvacuationStrategy
{
public:
	CSimulationStrategy();
	~CSimulationStrategy();

	void run() override;

private:
	void __initEvacuationDirection();
	void __updateEvacuationDirection();
	void __assignAgentNavNode();
	bool __isConverged() const;
	void __doSimulation();
	
private:
	std::vector<SimNode*> m_NodeSet;
	int m_MaxIterationNum = 0;
};