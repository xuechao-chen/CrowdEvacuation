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

private:
	void __init();
	void __avoidStuckInObstacles();
	virtual bool __isFinish() = 0;
	virtual void __onPreDoStep() = 0;
	virtual void __onPostDoStep() = 0;

protected:
	CEvacuationScene* m_pScene = nullptr;
	int m_EvacuationTimeCost = 0;
};