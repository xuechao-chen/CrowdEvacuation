#include "BaseStrategy.h"
#include "ConfigParser.h"
#include "SceneVis.h"

IEvacuationStrategy::IEvacuationStrategy()
{
}

IEvacuationStrategy::~IEvacuationStrategy()
{
	_SAFE_DELETE(m_pScene);
}

void IEvacuationStrategy::run()
{
	auto pSim = m_pScene->getSimulator();

	std::cout << "Evacuation Begin" << std::endl;

	do {
		m_EvacuationTimeCost++;
		__updateVisualization();
		__onPreDoStep();
 		pSim->doStep();
		__avoidStuckInObstacles();
		__onPostDoStep();
	} while (!__isFinish());

	std::cout << "Evacuation End" << std::endl;
	std::cout << "Evacuation time cost: " << m_EvacuationTimeCost << std::endl;
}

void IEvacuationStrategy::init()
{
	__constructEvacuationScene();
	__constructRoadMap();
	__assignNavNode2Agent();
}

void IEvacuationStrategy::__constructEvacuationScene()
{
	CSceneGraph* pGraph = new CSceneGraph();
	RVO::RVOSimulator* pSim = new RVO::RVOSimulator();

	CConfigParser::parseGraph("GraphConfig.xml", pGraph);
	CConfigParser::parseRVOSimulator("SimulatorConfig.xml", pSim);

	CEvacuationScene* pScene = new CEvacuationScene(pSim, pGraph);
	CConfigParser::parseScene("SceneConfig.xml", pScene);

	setEvacuationScene(pScene);
}

void IEvacuationStrategy::__updateVisualization()
{
	CSceneVis::getInstance()->displayScene(m_pScene);
}

void IEvacuationStrategy::__avoidStuckInObstacles()
{
	//TODO: 避免Agent卡墙
	//沿着倾向速度的方向前进一段距离
}

bool __isVisited(const glm::vec2& vNode, const std::vector<glm::vec2>& vVisitedNodeSet)
{
	for (auto& VisitedNode : vVisitedNodeSet)
	{
		if (vNode == VisitedNode) return true;
	}
	return false;
}

std::vector<glm::vec2> __findShortestPathToExit(const glm::vec2& vNode, const std::vector<glm::vec2>& vExits, CSceneGraph* pGraph)
{
	std::vector<glm::vec2> ShortestNavNodeSet;
	auto ShortestDistance = FLT_MAX;
	for (auto& Exit : vExits)
	{
		const auto& Path = pGraph->findShortestPath(vNode, Exit);
		auto& NavNodeSet = Path.first;
		auto Distance = Path.second;
		if (Distance < ShortestDistance)
		{
			ShortestNavNodeSet = NavNodeSet;
			ShortestDistance = Distance;
		}
	}
	return ShortestNavNodeSet;
}