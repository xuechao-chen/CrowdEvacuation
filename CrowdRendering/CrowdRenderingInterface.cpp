#include "CrowdRenderingInterface.h"
#include "SceneVisualization.h"

void hiveCrowdRendering::init(const std::string & vConfig)
{
	CSceneVisualization::getInstance()->init(vConfig);
}

void hiveCrowdRendering::replay(const std::string & ObstaclesPosFile, const std::string & AgentsPosFile)
{
	auto SceneVis = CSceneVisualization::getInstance();
	//TODO: 从文件中可视化
}

void hiveCrowdRendering::pause()
{
	CSceneVisualization::getInstance()->pause();
}

void hiveCrowdRendering::drawObstacles(const std::vector<std::pair<glm::vec2, glm::vec2>>& vPosOfObstacles)
{
	auto SceneVis = CSceneVisualization::getInstance();

	for (auto& Pos : vPosOfObstacles)
	{
		SceneVis->drawObstacle(Pos.first, Pos.second);
	}
}

void hiveCrowdRendering::drawAgents(const std::vector<glm::vec2>& vPosOfAgents)
{
	auto SceneVis = CSceneVisualization::getInstance();
	for (auto& Pos : vPosOfAgents)
	{
		SceneVis->drawAgent(Pos);
	}
}

void hiveCrowdRendering::drawNodes(const std::vector<glm::vec2>& vNodes)
{
	auto SceneVis = CSceneVisualization::getInstance();
	for (auto& Node : vNodes)
	{
		SceneVis->drawNode(Node);
	}
}

void hiveCrowdRendering::drawEdges(const std::vector<std::pair<glm::vec2, glm::vec2>>& vEdges)
{
	auto SceneVis = CSceneVisualization::getInstance();
	for (auto& Edge : vEdges)
	{
		SceneVis->drawEdge(Edge.first, Edge.second);
	}
}

void hiveCrowdRendering::display()
{
	auto SceneVis = CSceneVisualization::getInstance();
	SceneVis->display();
}

void hiveCrowdRendering::clear()
{
	auto SceneVis = CSceneVisualization::getInstance();
	SceneVis->clear();
}

void hiveCrowdRendering::saveImage(const char * vPath)
{
	auto SceneVis = CSceneVisualization::getInstance();
	SceneVis->saveImage(vPath);
}

void hiveCrowdRendering::saveVideo()
{
	auto SceneVis = CSceneVisualization::getInstance();
	SceneVis->saveVideo();
}
