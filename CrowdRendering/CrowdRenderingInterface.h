#pragma once
#include "CrowdRenderingExport.h"
#include <vector>
#include "GLM/glm.hpp"

namespace hiveCrowdRendering
{
	CROWD_RENDERING_DLL_EXPORT void init(const std::string& vConfig);
	CROWD_RENDERING_DLL_EXPORT void replay(const std::string& ObstaclesPosFile, const std::string& AgentsPosFile);
	CROWD_RENDERING_DLL_EXPORT void pause();
	CROWD_RENDERING_DLL_EXPORT void drawObstacles(const std::vector<std::pair<glm::vec2, glm::vec2>>& vPosOfObstacles);
	CROWD_RENDERING_DLL_EXPORT void drawAgents(const std::vector<glm::vec2>& vPosOfAgents);
	CROWD_RENDERING_DLL_EXPORT void drawNodes(const std::vector<glm::vec2>& vNodes);
	CROWD_RENDERING_DLL_EXPORT void drawEdges(const std::vector<std::pair<glm::vec2, glm::vec2>>& vEdges);
	CROWD_RENDERING_DLL_EXPORT void display();
	CROWD_RENDERING_DLL_EXPORT void clear();

	CROWD_RENDERING_DLL_EXPORT void saveImage(const char* vPath);
}