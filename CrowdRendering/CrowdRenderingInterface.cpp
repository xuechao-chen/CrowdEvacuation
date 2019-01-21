#include "CrowdRenderingInterface.h"
#include "SceneVisualization.h"

void hiveCrowdRendering::init(const std::string & vConfig)
{
	CSceneVisualization::getInstance()->init(vConfig);
}
                                                                                                                                      
void hiveCrowdRendering::draw(const std::vector<std::pair<glm::vec2, glm::vec2>>& vPosOfObstacles, const std::vector<glm::vec2>& vPosOfAgents)
{
	auto SceneVis = CSceneVisualization::getInstance();

	for (auto& Pos : vPosOfObstacles)
	{
		SceneVis->drawObstacle(Pos.first, Pos.second);
	}

	for (auto& Pos : vPosOfAgents)
	{
		SceneVis->drawAgent(Pos);
	}

	SceneVis->display();
}

void hiveCrowdRendering::replay(const std::string & ObstaclesPosFile, const std::string & AgentsPosFile)
{
	auto SceneVis = CSceneVisualization::getInstance();
	//TODO
}

void hiveCrowdRendering::pause()
{
	CSceneVisualization::getInstance()->pause();
}

CROWD_RENDERING_DLL_EXPORT void hiveCrowdRendering::drawObstacles(const std::vector<std::pair<glm::vec2, glm::vec2>>& vPosOfObstacles)
{
	auto SceneVis = CSceneVisualization::getInstance();

	for (auto& Pos : vPosOfObstacles)
	{
		SceneVis->drawObstacle(Pos.first, Pos.second);
	}
}

CROWD_RENDERING_DLL_EXPORT void hiveCrowdRendering::drawAgents(const std::vector<glm::vec2>& vPosOfAgents)
{
	auto SceneVis = CSceneVisualization::getInstance();
	for (auto& Pos : vPosOfAgents)
	{
		SceneVis->drawAgent(Pos);
	}
}

CROWD_RENDERING_DLL_EXPORT void hiveCrowdRendering::display()
{
	auto SceneVis = CSceneVisualization::getInstance();
	SceneVis->display();

}

CROWD_RENDERING_DLL_EXPORT void hiveCrowdRendering::clear()
{
	auto SceneVis = CSceneVisualization::getInstance();
	SceneVis->clear();
}
