#pragma once
#include "../CrowdRendering/CrowdRenderingInterface.h"
#include "EvacuationScene.h"
#include "common/Singleton.h"

class CSceneVis : public hiveOO::CSingleton<CSceneVis>
{
public:
	CSceneVis();
	~CSceneVis();

	void displayScene(const CEvacuationScene* pScene);
	void pause();
	void saveImage(const char* vPath);
	void saveVideo();

private:
	friend class hiveOO::CSingleton<CSceneVis>;
};