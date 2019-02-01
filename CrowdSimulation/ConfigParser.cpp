#include "ConfigParser.h"
#include "common/ConfigInterface.h"

CConfigParser::CConfigParser()
{
}

CConfigParser::~CConfigParser()
{
}

void CConfigParser::parseGraph(const std::string & vConfig, CSceneGraph * voGraph)
{
	hiveConfig::hiveParseConfig(vConfig, hiveConfig::EConfigType::XML, CGraphConfig::getInstance());
	auto Config = CGraphConfig::getInstance();

	__parseNodes(Config, voGraph);
	__parseEdges(Config, voGraph);
}

void CConfigParser::parseScene(const std::string & vConfig, CEvacuationScene* voScene)
{
	hiveConfig::hiveParseConfig(vConfig, hiveConfig::EConfigType::XML, CSceneConfig::getInstance());
	auto Config = CSceneConfig::getInstance();

	__parseObstacles(Config, voScene);
	__parseExits(Config, voScene);
	__parseAgents(Config, voScene);
}

void CConfigParser::parseRVOSimulator(const std::string & vConfig, RVO::RVOSimulator * voSimulator)
{
	hiveConfig::hiveParseConfig(vConfig, hiveConfig::EConfigType::XML, CSimulatorConfig::getInstance());
	auto Config = CSimulatorConfig::getInstance();

	auto TimeStep = Config->getAttribute<float>(KEY_WORDS::TIMESTEP);
	auto NeighborDist = Config->getAttribute<float>(KEY_WORDS::NEIGHBOR_DIST);
	auto MaxNeighbors = Config->getAttribute<int>(KEY_WORDS::MAX_NEIGHBORS);
	auto TimeHorizon = Config->getAttribute<float>(KEY_WORDS::TIME_HORIZON);
	auto TimeHorizonObst = Config->getAttribute<float>(KEY_WORDS::TIME_HORIZON_OBST);
	auto Radius = Config->getAttribute<float>(KEY_WORDS::RADIUS);
	auto MaxSpeed = Config->getAttribute<float>(KEY_WORDS::MAX_SPEED);

	voSimulator->setTimeStep(TimeStep);
	voSimulator->setAgentDefaults(NeighborDist, MaxNeighbors, TimeHorizon, TimeHorizonObst, Radius, MaxSpeed);
}

void CConfigParser::__genAgentInRegion(int vAmount, int* vRegion, std::vector<glm::vec2>& voAgentsInRegion)
{
	auto Radius = CSimulatorConfig::getInstance()->getAttribute<float>(KEY_WORDS::RADIUS);

	for (size_t i = 0; i < vAmount;)
	{
		auto RangeX = vRegion[2] - vRegion[0] - 2*Radius;
		auto RangeY = vRegion[3] - vRegion[1] - 2*Radius;

		float PosX = (rand() / float(RAND_MAX)) * RangeX + vRegion[0] + Radius;
		float PosY = (rand() / float(RAND_MAX)) * RangeY + vRegion[1] + Radius;

		if (!__checkCollison({PosX, PosY}, Radius, voAgentsInRegion))
		{
			voAgentsInRegion.push_back({PosX, PosY});
			++i;
		}
	}
}

bool CConfigParser::__checkCollison(const glm::vec2& vPos, float vRadius, std::vector<glm::vec2>& voAgentsInRegion)
{
	for (auto Agent : voAgentsInRegion)
	{
		auto CollisonRange = 2 * vRadius;
		if (glm::distance(vPos, Agent) <= CollisonRange)
		{
			return true;
		}
	}
	return false;
}

void CConfigParser::__parseNodes(CGraphConfig * pConfig, CSceneGraph * voGraph)
{
	auto NumNode = pConfig->getAttribute<int>(KEY_WORDS::NODE_NUM);
	std::string NodeSetStr = pConfig->getAttribute<std::string>(KEY_WORDS::NODE);

	int* NodeSetPos = new int[NumNode * 2];
	hiveCommon::hiveSplitLine2IntArray(NodeSetStr, " ", NumNode * 2, NodeSetPos);
	for (size_t i = 0; i < NumNode; i++)
	{
		voGraph->addNode({ NodeSetPos[2 * i], NodeSetPos[2 * i + 1] });
	}
}

void CConfigParser::__parseEdges(CGraphConfig * pConfig, CSceneGraph * voGraph)
{
	auto NumEdge = pConfig->getAttribute<int>(KEY_WORDS::EDGE_NUM);
	std::string EdgeSetStr = pConfig->getAttribute<std::string>(KEY_WORDS::EDGE);

	int* EdgeSetPos = new int[NumEdge * 4];
	hiveCommon::hiveSplitLine2IntArray(EdgeSetStr, " ", NumEdge * 4, EdgeSetPos);
	for (size_t i = 0; i < NumEdge; i++)
	{
		glm::vec2 Node1 = { EdgeSetPos[4 * i], EdgeSetPos[4 * i + 1] };
		glm::vec2 Node2 = { EdgeSetPos[4 * i + 2], EdgeSetPos[4 * i + 3] };
		double Weight = glm::distance(Node1, Node2);
		voGraph->addEdge(Node1, Node2, Weight);
	}
}

void CConfigParser::__parseAgents(CSceneConfig * pConfig, CEvacuationScene * voScene)
{
	for (size_t i = 0; i < pConfig->getNumSubConfig(); i++)
	{
		auto SubConfig = pConfig->getSubConfigAt(i);
		auto RegionStr = SubConfig->getAttribute<std::string>(KEY_WORDS::REGION);
		int* Region = new int[4];
		hiveCommon::hiveSplitLine2IntArray(RegionStr, " ", 4, Region);
		auto Amount = SubConfig->getAttribute<int>(KEY_WORDS::AMOUNT);

		std::vector<glm::vec2> AgentsInRegion;
		__genAgentInRegion(Amount, Region, AgentsInRegion);
		for (auto Agent : AgentsInRegion)
		{
			voScene->addAgent(Agent);
		}
	}
}

void CConfigParser::__parseObstacles(CSceneConfig * pConfig, CEvacuationScene * voScene)
{
	auto NumObstacles = pConfig->getAttribute<int>(KEY_WORDS::OBSTACLE_NUM);
	std::string ObstacleSetStr = pConfig->getAttribute<std::string>(KEY_WORDS::OBSTACLE);

	int* ObstacleSetPos = new int[NumObstacles * 4];
	hiveCommon::hiveSplitLine2IntArray(ObstacleSetStr, " ", NumObstacles * 4, ObstacleSetPos);
	for (size_t i = 0; i < NumObstacles; i++)
	{
		glm::vec2 LeftTop = { ObstacleSetPos[4 * i], ObstacleSetPos[4 * i + 1] };
		glm::vec2 RightBottom = { ObstacleSetPos[4 * i + 2], ObstacleSetPos[4 * i + 3] };
		voScene->addObstacle(LeftTop, RightBottom);
	}
}

void CConfigParser::__parseExits(CSceneConfig * pConfig, CEvacuationScene * voScene)
{
	auto NumExits = pConfig->getAttribute<int>(KEY_WORDS::EXIT_NUM);
	std::string ExitSetStr = pConfig->getAttribute<std::string>(KEY_WORDS::EXIT);

	int* ExitSetPos = new int[NumExits * 2];
	hiveCommon::hiveSplitLine2IntArray(ExitSetStr, " ", NumExits * 2, ExitSetPos);
	for (size_t i = 0; i < NumExits; i++)
	{
		voScene->addExit({ ExitSetPos[2 * i], ExitSetPos[2 * i + 1] });
	}
}