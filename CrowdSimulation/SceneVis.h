#pragma once
#include "../CrowdRendering/CrowdRenderingInterface.h"
#include "EvacuationScene.h"
#include "common/Singleton.h"

class CSceneVis : public hiveOO::CSingleton<CSceneVis>
{
public:
	~CSceneVis();

	void displayScene(const CEvacuationScene* pScene);

private:
	CSceneVis();

	friend class hiveOO::CSingleton<CSceneVis>;
};

