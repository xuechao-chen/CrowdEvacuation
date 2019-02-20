#include "../CrowdSimulation/EvacuationScene.h"
#include "../CrowdSimulation/SceneVis.h"
#include "../CrowdSimulation/ConfigParser.h"
#include "../CrowdSimulation/ShortestStrategy.h"

int main(int, char*[])
{
	CSceneGraph* pGraph = new CSceneGraph();
	RVO::RVOSimulator* pSim = new RVO::RVOSimulator();

	CConfigParser::parseGraph("GraphConfig.xml", pGraph);
	CConfigParser::parseRVOSimulator("SimulatorConfig.xml", pSim);

	CEvacuationScene* pScene = new CEvacuationScene(pSim, pGraph);
	CConfigParser::parseScene("SceneConfig.xml", pScene);

	CSceneVis::getInstance()->displayScene(pScene);
	CSceneVis::getInstance()->pause();

	delete pScene;

	return 0;
}