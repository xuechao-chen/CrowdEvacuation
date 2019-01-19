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
	
}

void CSimulationStrategy::__initPaths()
{
	auto Adapter = m_pScene->getGraphAdapter();
	auto AllNodePos = Adapter->dumpAllNodePos();

	std::set<int> VisitedNode;
	for (auto& NodePos : AllNodePos)
	{
		auto NodeIdx = Adapter->getIdx4Pos(NodePos);
		if (VisitedNode.find(NodeIdx) == VisitedNode.end()) continue;

		auto Exits = m_pScene->getExits();
		for (auto& Exit : Exits)
		{
			if (NodeIdx == Adapter->getIdx4Pos(Exit)) continue;

			auto Path = Adapter->findShortestPath(NodePos, Exit);
			for (auto& NodeOfPath : Path) VisitedNode.insert(Adapter->getIdx4Pos(NodeOfPath));

		}
	}
}

void CSimulationStrategy::__updatePaths()
{
}
