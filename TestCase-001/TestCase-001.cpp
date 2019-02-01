#include <iostream>
#include "../CrowdRendering/CrowdRenderingInterface.h"

#include "../CrowdSimulation/EvacuationScene.h"
#include "../CrowdSimulation/SceneVis.h"
#include "../CrowdSimulation/ConfigParser.h"

int main(int, char*[])
{
	CSceneGraph* pGraph = new CSceneGraph();
	RVO::RVOSimulator* pSim = new RVO::RVOSimulator();

	CConfigParser::parseGraph("GraphConfig.xml", pGraph);
	CConfigParser::parseRVOSimulator("SimulatorConfig.xml", pSim);

	CEvacuationScene* pScene = new CEvacuationScene(pSim, pGraph);
	CConfigParser::parseScene("SceneConfig.xml", pScene);

	CSceneVis::getInstance()->displayScene(pScene);

	return 0;
}