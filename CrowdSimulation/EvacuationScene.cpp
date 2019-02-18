#include "EvacuationScene.h"
#include "common/HiveCommonMicro.h"

CEvacuationScene::~CEvacuationScene()
{
	_SAFE_DELETE(m_pGraph);
	_SAFE_DELETE(m_pRVOSimulator);
}

void CEvacuationScene::addObstacle(const glm::vec2 & vLeftTop, const glm::vec2 & vRightBottom)
{
	m_Obstacles.push_back({ vLeftTop, vRightBottom });

	std::vector<RVO::Vector2> Vertices;
	Vertices.push_back(RVO::Vector2(vLeftTop.x, vLeftTop.y));
	Vertices.push_back(RVO::Vector2(vRightBottom.x, vLeftTop.y));
	Vertices.push_back(RVO::Vector2(vRightBottom.x, vRightBottom.y));
	Vertices.push_back(RVO::Vector2(vLeftTop.x, vRightBottom.y));
	m_pRVOSimulator->addObstacle(Vertices);

	//m_pRVOSimulator->processObstacles();
	//NOTE: Process obstacles so that they are accounted for in the simulation.
}

void CEvacuationScene::addExit(const glm::vec2 & vExit)
{
	m_Exits.push_back(vExit);
}

void CEvacuationScene::removeExit(const glm::vec2 & vExit)
{
	for (auto it = m_Exits.begin(); it != m_Exits.end(); ++it)
	{
		if (vExit == *it)
		{
			m_Exits.erase(it);
			break;
		}
	}
}

std::vector<IAgent*> CEvacuationScene::dumpAgentsInRegion(const glm::vec2 & vLeftTop, const glm::vec2 & vRightBottom)
{
	std::vector<IAgent*> AgentsInRegion;
	for (auto Agent : m_Agents)
	{
		const auto& Pos = Agent->getInitPosition();
		if ((Pos.x > vLeftTop.x && Pos.x < vRightBottom.x && Pos.y > vLeftTop.y && Pos.y < vRightBottom.y) ||
			(Pos.x > vRightBottom.x && Pos.x < vLeftTop.x && Pos.y > vRightBottom.y && Pos.y < vLeftTop.y))
		{
			AgentsInRegion.push_back(Agent);
		}
	}
	return AgentsInRegion;
}

std::vector<IAgent*> CEvacuationScene::dumpAgentsInNode(const glm::vec2 & vNode)
{
	return dumpAgentsInRegion(glm::vec2(vNode.x - CSceneGraph::ROAD_WIDTH / 2, vNode.y - CSceneGraph::ROAD_WIDTH / 2),
							  glm::vec2(vNode.x + CSceneGraph::ROAD_WIDTH / 2, vNode.y + CSceneGraph::ROAD_WIDTH / 2));
}

std::vector<IAgent*> CEvacuationScene::dumpAgentsInEdge(const glm::vec2 & vNode1, const glm::vec2 & vNode2)
{
	if (vNode1.x == vNode2.x)
	{
		return dumpAgentsInRegion(glm::vec2(vNode1.x - CSceneGraph::ROAD_WIDTH / 2, vNode1.y), 
			                      glm::vec2(vNode2.x + CSceneGraph::ROAD_WIDTH / 2, vNode2.y));
	}
	else
	{
		return dumpAgentsInRegion(glm::vec2(vNode1.x, vNode1.y - CSceneGraph::ROAD_WIDTH / 2),
								  glm::vec2(vNode2.x, vNode2.y + CSceneGraph::ROAD_WIDTH / 2));
	}
}

void CEvacuationScene::addAgent(const glm::vec2& vAgent)
{
	auto Idx = m_pRVOSimulator->addAgent(RVO::Vector2(vAgent.x, vAgent.y));
	auto RVOAgent = m_pRVOSimulator->getAgent(Idx);
	m_Agents.push_back(new IAgent(RVOAgent, vAgent));//TODO ∂‡Ã¨Agent
}
