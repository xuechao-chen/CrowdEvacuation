#pragma once
#include "EvacuationScene.h"
#include "../RVO/RVOSimulator.h"

//NOTE: FOR UNIT TEST
#ifdef _DEBUG
#define private public
#define protected public
#endif

// 人群在场景中的运动和疏散算法
class IEvacuationStrategy
{
public:
	IEvacuationStrategy();
	virtual ~IEvacuationStrategy();

	void setEvacuationScene(CEvacuationScene* pScene) { m_pScene = pScene; }
	CEvacuationScene* getEvacuationScene() const { return m_pScene; }
	virtual void run();
	virtual void init();

private:
	virtual bool __isFinish() = 0;
	virtual void __onPreDoStep() = 0;
	virtual void __onPostDoStep() = 0;
	virtual void __constructRoadMap() = 0;
	virtual void __assignNavNode2Agent() = 0;

	void __constructEvacuationScene();
	void __updateVisualization();

protected:
	CEvacuationScene* m_pScene = nullptr;
	int m_EvacuationTimeCost = 0;
};

bool __isVisited(const glm::vec2& vNode, const std::vector<glm::vec2>& vVisitedNodeSet);
std::vector<glm::vec2> __findShortestPathToExit(const glm::vec2& vNode, const std::vector<glm::vec2>& vExits, CSceneGraph* pGraph);