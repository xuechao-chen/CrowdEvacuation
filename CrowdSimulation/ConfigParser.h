#pragma once
#include "GraphConfig.h"
#include "SceneGraph.h"
#include "SceneConfig.h"
#include "EvacuationScene.h"
#include "SimulatorConfig.h"
#include "../RVO/RVOSimulator.h"

class CConfigParser
{
public:
	CConfigParser();
	~CConfigParser();

	static void parseGraph(const std::string& vConfig, CSceneGraph* voGraph);
	static void parseScene(const std::string& vConfig, CEvacuationScene* voScene);
	static void parseRVOSimulator(const std::string& vConfig, RVO::RVOSimulator* voSimulator);

private:
	static void __genAgentInRegion(int vAmount, int* vRegion, std::vector<glm::vec2>& voAgentsInRegion);
	static bool __checkCollison(const glm::vec2 & vPos, float vRadius, std::vector<glm::vec2>& voAgentsInRegion);
	
	static void __parseNodes(CGraphConfig* pConfig, CSceneGraph* voGraph);
	static void __parseEdges(CGraphConfig* pConfig, CSceneGraph* voGraph);
	static void __parseAgents(CSceneConfig* pConfig, CEvacuationScene* voScene);
	static void __parseObstacles(CSceneConfig* pConfig, CEvacuationScene* voScene);
	static void __parseExits(CSceneConfig* pConfig, CEvacuationScene* voScene);
};