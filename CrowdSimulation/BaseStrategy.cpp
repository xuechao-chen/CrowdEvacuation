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

float __calAvgEvacuationTime4Agents(const std::vector<IAgent*>& vAgents)
{
	float TotalTime = 0.0f;
	for (auto Agent : vAgents)
	{
		TotalTime += Agent->getEvacuationTime();
	}
	return TotalTime / vAgents.size();
}

float __calMaxEvacuationTime4Agents(const std::vector<IAgent*>& vAgents)
{
	float MaxTime = -FLT_MAX;
	for (auto Agent : vAgents)
	{
		auto EvacuationTime = Agent->getEvacuationTime();
		if (EvacuationTime > MaxTime) MaxTime = EvacuationTime;
	}
	return MaxTime;
}
