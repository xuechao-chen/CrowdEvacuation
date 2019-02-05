#include "SimulationStrategy.h"
#include <set>

CSimulationStrategy::CSimulationStrategy()
{
}

CSimulationStrategy::~CSimulationStrategy()
{
}

bool CSimulationStrategy::__isFinish()
{
	if (m_IsConverged || m_IterationNum >= 10)
	{
		return true;
	}
	return false;
}

void CSimulationStrategy::__onPreDoStep()
{
	
}

void CSimulationStrategy::__onPostDoStep()
{
	auto IsAllAgentReachExit = true;
	const auto& Agents = m_pScene->getAgents();
	const auto& Exits = m_pScene->getExits();
	for (auto& Agent : Agents)
	{
		if (!Agent->isReachExit(Exits))
		{
			IsAllAgentReachExit = false;
			break;
		}
	}

	if (IsAllAgentReachExit)
	{
		m_IterationNum++;
		__analyzeConvergence();
		if (!m_IsConverged)
		{
			//TODO: reset scene and agent
		}
	}
}

void CSimulationStrategy::__constructRoadMap()
{
}

void CSimulationStrategy::__assignNavNode2Agent()
{
}

void CSimulationStrategy::__analyzeConvergence()
{
	if (__isDivideNodeConverged() && __isDistributionNodeConverged())
	{
		m_IsConverged = true;
	}
}

bool CSimulationStrategy::__isDivideNodeConverged()
{
	
}

bool CSimulationStrategy::__isDistributionNodeConverged()
{
}