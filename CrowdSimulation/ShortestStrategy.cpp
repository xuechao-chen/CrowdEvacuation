#include "ShortestStrategy.h"

void CShortestStrategy::__afterSimulationDoStep()
{
	const auto& Agents = m_pScene->getAgents();
	for (auto& Agent : Agents)
	{
		if (Agent->isReachNavNode())
		{
			const auto& CurNavNode = Agent->getNavNode();
			auto NextNavNode = m_RoadMap[CurNavNode];

			//NOTE: 当前导航点为出口，则下个导航点仍为出口
			if (NextNavNode == glm::vec2(FLT_MAX, FLT_MAX)) NextNavNode = CurNavNode;

			Agent->setNavNode(NextNavNode);
			glm::vec2 Direction = NextNavNode - Agent->getPosition();
			Agent->setPrefVelocity(glm::normalize(Direction));
		}
	}
}