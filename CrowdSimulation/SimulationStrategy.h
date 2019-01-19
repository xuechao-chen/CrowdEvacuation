#pragma once
#include "BaseEvacuationStrategy.h"

class CSimulationStrategy : public IEvacuationStrategy
{
public:
	CSimulationStrategy();
	~CSimulationStrategy();

	void run() override;

private:
	
private:
	int m_IterationTime;
};

