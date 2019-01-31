#include "ConfigParser.h"
#include "GraphConfig.h"
#include "SimulatorConfig.h"
#include "AgentConfig.h"
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

	auto NumNode = CGraphConfig::getInstance()->getAttribute<int>(KEY_WORDS::NODE_NUM);
	auto NumEdge = CGraphConfig::getInstance()->getAttribute<int>(KEY_WORDS::EDGE_NUM);
	std::string NodeSetStr = CGraphConfig::getInstance()->getAttribute<std::string>(KEY_WORDS::NODE);
	std::string EdgeSetStr = CGraphConfig::getInstance()->getAttribute<std::string>(KEY_WORDS::EDGE);

	int* NodeSetPos = new int[NumNode * 2];
	hiveCommon::hiveSplitLine2IntArray(NodeSetStr, " ", NumNode*2, NodeSetPos);
	for (size_t i = 0; i < NumNode; i++)
	{
		voGraph->addNode({NodeSetPos[2*i], NodeSetPos[2*i+1]});
	}

	int* EdgeSetPos = new int[NumEdge * 4];
	hiveCommon::hiveSplitLine2IntArray(EdgeSetStr, " ", NumEdge*4, EdgeSetPos);
	for (size_t i = 0; i < NumEdge; i++)
	{
		glm::vec2 Node1 = { EdgeSetPos[4*i], EdgeSetPos[4*i+1] };
		glm::vec2 Node2 = { EdgeSetPos[4*i+2], EdgeSetPos[4*i+3] };
		double Weight = glm::distance(Node1, Node2);
		voGraph->addEdge(Node1, Node2, Weight);
	}
}

void CConfigParser::parseRVOSimulator(const std::string & vConfig, RVO::RVOSimulator * voSimulator)
{
	hiveConfig::hiveParseConfig(vConfig, hiveConfig::EConfigType::XML, CSimulatorConfig::getInstance());
	auto Config = CSimulatorConfig::getInstance();

	auto TimeStep        = Config->getAttribute<float>(KEY_WORDS::TIMESTEP);
	auto NeighborDist    = Config->getAttribute<float>(KEY_WORDS::NEIGHBOR_DIST);
	auto MaxNeighbors    = Config->getAttribute<int>(KEY_WORDS::MAX_NEIGHBORS);
	auto TimeHorizon     = Config->getAttribute<float>(KEY_WORDS::TIME_HORIZON);
	auto TimeHorizonObst = Config->getAttribute<float>(KEY_WORDS::TIME_HORIZON_OBST);
	auto Radius          = Config->getAttribute<float>(KEY_WORDS::RADIUS);
	auto MaxSpeed        = Config->getAttribute<float>(KEY_WORDS::MAX_SPEED);

	voSimulator->setTimeStep(TimeStep);
	voSimulator->setAgentDefaults(NeighborDist, MaxNeighbors, TimeHorizon, TimeHorizonObst, Radius, MaxSpeed);
}

void CConfigParser::parseAgent(const std::string & vConfig, RVO::RVOSimulator* vSimulator, std::vector<IAgent*>& voAgentSet)
{
	hiveConfig::hiveParseConfig(vConfig, hiveConfig::EConfigType::XML, CAgentConfig::getInstance());
	auto Config = CAgentConfig::getInstance();

	for (size_t i = 0; i < Config->getNumSubConfig(); i++)
	{
		auto SubConfig = Config->getSubConfigAt(i);
		auto RegionStr = SubConfig->getAttribute<std::string>(KEY_WORDS::REGION);
		int* Region = new int[4];
		hiveCommon::hiveSplitLine2IntArray(RegionStr, " ", 4, Region);
		auto Amount = SubConfig->getAttribute<int>(KEY_WORDS::AMOUNT);
		
		std::vector<IAgent*> AgentsInRegion;
		__genAgentInRegion(Amount, Region, vSimulator, AgentsInRegion);
		for (auto Agent : AgentsInRegion) voAgentSet.push_back(Agent);
	}
}

void CConfigParser::__genAgentInRegion(int vAmount, int* vRegion, RVO::RVOSimulator* vSimulator, std::vector<IAgent*>& voAgentsInRegion)
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
			//TODO ¶àÌ¬Agent
			auto Idx = vSimulator->addAgent(RVO::Vector2(PosX, PosY));
			IAgent* pAgent = new IAgent(vSimulator->getAgent(Idx), {PosX, PosY});
			voAgentsInRegion.push_back(pAgent);
			++i;
		}
	}
}

bool CConfigParser::__checkCollison(const glm::vec2& vPos, float vRadius, std::vector<IAgent*>& voAgentsInRegion)
{
	for (auto Agent : voAgentsInRegion)
	{
		auto CollisonRange = 2 * vRadius;
		if (glm::distance(vPos, Agent->getPosition()) <= CollisonRange)
		{
			return true;
		}
	}
	return false;
}