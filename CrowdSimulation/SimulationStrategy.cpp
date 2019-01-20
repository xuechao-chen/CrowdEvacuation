#include "SimulationStrategy.h"
#include <set>

CSimulationStrategy::CSimulationStrategy()
{
}

CSimulationStrategy::~CSimulationStrategy()
{
}

void CSimulationStrategy::run()
{
	auto Count = 0;
	__initEvacuationDirection();
	do
	{
		__assignAgentNavNode();
		__doSimulation();
		if (__isConverged()) break;
		__updateEvacuationDirection();
	} while (Count == m_MaxIterationNum);
}

void CSimulationStrategy::__initEvacuationDirection()
{
	auto Graph = m_pScene->getGraph();
}

void CSimulationStrategy::__updateEvacuationDirection()
{
}

void CSimulationStrategy::__assignAgentNavNode()
{
}

bool CSimulationStrategy::__isConverged() const
{
	return false;
}

void CSimulationStrategy::__doSimulation()
{
}
