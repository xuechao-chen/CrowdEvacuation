#pragma once
#include "EvacuationScene.h"
#include "../RVO/RVOSimulator.h"

//NOTE: FOR UNIT TEST
#ifdef _DEBUG
#define private public
#define protected public
#endif

class IEvacuationStrategy
{
public:
	IEvacuationStrategy();
	virtual ~IEvacuationStrategy();

	virtual void run();
	virtual void init();
	void setEvacuationScene(CEvacuationScene* pScene) { m_pScene = pScene; }
	CEvacuationScene* getEvacuationScene() const { return m_pScene; }

private:
	virtual bool __isFinish() = 0;
	virtual void __afterSimulationDoStep() = 0;
	virtual void __constructRoadMap() = 0;
	virtual void __assignNavNode2Agent() = 0;

	void __constructEvacuationScene();
	void __updateVisualization();

protected:
	int m_EvacuationTimeCost = 0;
	CEvacuationScene* m_pScene = nullptr;
};

bool __isVisited(const glm::vec2& vNode, const std::vector<glm::vec2>& vVisitedNodeSet);
std::pair<std::vector<glm::vec2>, float> __findShortestPathToExit(const glm::vec2& vNode, const std::vector<glm::vec2>& vExits, CSceneGraph* pGraph);
float __calAvgEvacuationTime4Agents(const std::vector<IAgent*>& vAgents);
float __calMaxEvacuationTime4Agents(const std::vector<IAgent*>& vAgents);