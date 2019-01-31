#pragma once
#include "SceneGraph.h"
#include "../RVO/RVOSimulator.h"
#include "BaseAgent.h"

class CConfigParser
{
public:
	CConfigParser();
	~CConfigParser();

	static void parseGraph(const std::string& vConfig, CSceneGraph* voGraph);
	static void parseRVOSimulator(const std::string& vConfig, RVO::RVOSimulator* voSimulator);
	static void parseAgent(const std::string& vConfig, RVO::RVOSimulator* vSimulator, std::vector<IAgent*>& voAgentSet);

private:
	static void __genAgentInRegion(int vAmount, int * vRegion, RVO::RVOSimulator* vSimulator, std::vector<IAgent*>& voAgentsInRegion);
	static bool __checkCollison(const glm::vec2 & vPos, float vRadius, std::vector<IAgent*>& voAgentsInRegion);
};