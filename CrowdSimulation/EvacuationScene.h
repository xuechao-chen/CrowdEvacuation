#pragma once
#include <vector>
#include "GLM/glm.hpp"
#include "SceneGraph.h"
#include "BaseAgent.h"

class CEvacuationScene
{
public:
	CEvacuationScene() = delete;
	CEvacuationScene(RVO::RVOSimulator* pSimulator) : m_pRVOSimulator(pSimulator) {}
	CEvacuationScene(RVO::RVOSimulator* pSimulator, CSceneGraph* pGraph) : m_pRVOSimulator(pSimulator), m_pGraph(pGraph) {}
	~CEvacuationScene();

	std::vector<IAgent*> getAgents() const { return m_Agents; }
	void addAgent(const glm::vec2& vAgent);
	std::vector<std::pair<glm::vec2, glm::vec2>> getObstacles() const { return m_Obstacles; }
	void addObstacle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom);
	
	std::vector<glm::vec2> getExits() const { return m_Exits; }
	void addExit(const glm::vec2& vExit);
	void removeExit(const glm::vec2& vExit);

	void setGraph(CSceneGraph* pGraph) { m_pGraph = pGraph; }
	CSceneGraph* getGraph() const { return m_pGraph; }

	void setSimulator(RVO::RVOSimulator* pSimulator) { m_pRVOSimulator = pSimulator; }
	RVO::RVOSimulator* getSimulator() const { return m_pRVOSimulator; }

private:
	std::vector<std::pair<glm::vec2, glm::vec2>> m_Obstacles;
	std::vector<glm::vec2> m_Exits;
	std::vector<IAgent*> m_Agents;
	CSceneGraph* m_pGraph = nullptr;
	RVO::RVOSimulator* m_pRVOSimulator = nullptr;
};