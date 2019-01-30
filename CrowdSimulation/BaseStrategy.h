#pragma once
#include "EvacuationScene.h"
#include "BaseAgent.h"
#include "../RVO/RVOSimulator.h"

// 人群在场景中的运动和疏散算法
class IEvacuationStrategy
{
public:
	IEvacuationStrategy();
	virtual ~IEvacuationStrategy();

	virtual void run() = 0;

private:
	virtual void __init(const std::string& vSceneConfig, const std::string& vAgentConfig);
	void __initScene(const std::string& vSceneConfig);
	void __initAgents(const std::string& vAgentConfig);

protected:
	std::vector<IAgent*> m_Agents;
	CEvacuationScene* m_pScene = nullptr;
	RVO::RVOSimulator* m_pSimulator = nullptr;
};