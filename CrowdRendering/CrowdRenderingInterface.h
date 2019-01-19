#pragma once
#include "CrowdRenderingExport.h"
#include <vector>
#include "GLM/glm.hpp"

namespace hiveCrowdRendering
{
	CROWD_RENDERING_DLL_EXPORT void init(const std::string& vConfig);
	CROWD_RENDERING_DLL_EXPORT void draw(const std::vector<std::pair<glm::vec2, glm::vec2>>& vPosOfObstacles, const std::vector<glm::vec2>& vPosOfAgents);
	CROWD_RENDERING_DLL_EXPORT void replay(const std::string& ObstaclesPosFile, const std::string& AgentsPosFile);
	CROWD_RENDERING_DLL_EXPORT void pause();
}