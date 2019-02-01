#include "BaseStrategy.h"
#include "ConfigParser.h"

IEvacuationStrategy::IEvacuationStrategy()
{
	__init();
}

IEvacuationStrategy::~IEvacuationStrategy()
{
	_SAFE_DELETE(m_pScene);
}

void IEvacuationStrategy::run()
{
	auto pSim = m_pScene->getSimulator();

	do {
		__onPreDoStep();
		pSim->doStep();
		__onPostDoStep();
	} while (!__isFinish());
}

void IEvacuationStrategy::__init()
{
	CSceneGraph* pGraph = new CSceneGraph();
	RVO::RVOSimulator* pSim = new RVO::RVOSimulator();

	CConfigParser::parseGraph("GraphConfig.xml", pGraph);
	CConfigParser::parseRVOSimulator("SimulatorConfig.xml", pSim);

	CEvacuationScene* pScene = new CEvacuationScene(pSim, pGraph);
	CConfigParser::parseScene("SceneConfig.xml", pScene);
}