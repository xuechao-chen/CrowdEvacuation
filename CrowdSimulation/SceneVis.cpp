#include "SceneVis.h"

CSceneVis::CSceneVis()
{
	hiveCrowdRendering::init("RenderingConfig.xml");
}

CSceneVis::~CSceneVis()
{
}

void CSceneVis::displayScene(const CEvacuationScene* pScene)
{
	const auto& Obstacles = pScene->getObstacles();
	const auto& Agents = pScene->getAgents();
	const auto& Exits = pScene->getExits();

	std::vector<glm::vec2> AgentsPos;
	for (auto& Agent : Agents)
	{
		AgentsPos.push_back(Agent->getPosition());
	}

	hiveCrowdRendering::clear();
	hiveCrowdRendering::drawAgents(AgentsPos);
	hiveCrowdRendering::drawObstacles(Obstacles);
	hiveCrowdRendering::display();
}