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
	
private:
	std::vector<ISimNode*> m_NodeSet;
};