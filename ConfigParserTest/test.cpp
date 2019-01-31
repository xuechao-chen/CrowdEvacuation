#include "pch.h"
#include "../CrowdSimulation/ConfigParser.h"

TEST(TestConfigParser, ParseGraph)
{
	std::vector<glm::vec2> NodeSet = { {1,1},{2,2},{3,3},{4,4},{5,5} };

	CSceneGraph* pGraph = new CSceneGraph();
	CConfigParser::parseGraph("GraphConfig.xml", pGraph);

	EXPECT_EQ(5, pGraph->getNumNodes());
	EXPECT_EQ(5, pGraph->getNumEdges());

	auto& AdjNodeSet = pGraph->dumpAdjNodeSet(NodeSet[0]);
	ASSERT_EQ(2, AdjNodeSet.size());
	EXPECT_EQ(NodeSet[1], AdjNodeSet[0].first);
	EXPECT_EQ(NodeSet[2], AdjNodeSet[1].first);

	auto Weight = pGraph->getEdgeWeight(NodeSet[0], NodeSet[1]);
	EXPECT_NEAR(sqrt(2), Weight, 0.001f);
	
	delete pGraph;
}

TEST(TestConfigParser, ParseRVOSimulator)
{
	RVO::RVOSimulator* pSimulator = new RVO::RVOSimulator();
	CConfigParser::parseRVOSimulator("SimulatorConfig.xml", pSimulator);
	auto Idx = pSimulator->addAgent(RVO::Vector2(10.0f, 10.0f));
	
	EXPECT_EQ(30, pSimulator->getAgentNeighborDist(Idx));
	EXPECT_EQ(5, pSimulator->getAgentMaxNeighbors(Idx));
	EXPECT_EQ(2, pSimulator->getAgentTimeHorizon(Idx));
	EXPECT_EQ(2, pSimulator->getAgentTimeHorizonObst(Idx));
	EXPECT_EQ(3, pSimulator->getAgentRadius(Idx));
	EXPECT_EQ(4, pSimulator->getAgentMaxSpeed(Idx));

	delete pSimulator;
}

TEST(TestConfigParser, ParseAgent)
{
	RVO::RVOSimulator* pSimulator = new RVO::RVOSimulator();
	CConfigParser::parseRVOSimulator("SimulatorConfig.xml", pSimulator);

	std::vector<IAgent*> AgentSet;
	CConfigParser::parseAgent("AgentConfig.xml", pSimulator, AgentSet);

	EXPECT_EQ(15, AgentSet.size());

	for (size_t i = 0; i < 10; i++)
	{
		auto& Pos = AgentSet[i]->getPosition();
		EXPECT_LE(100, Pos.x);
		EXPECT_GE(200, Pos.x);
		EXPECT_LE(100, Pos.y);
		EXPECT_GE(200, Pos.y);
	}
	for (size_t i = 10; i < 15; i++)
	{
		auto& Pos = AgentSet[i]->getPosition();
		EXPECT_LE(300, Pos.x);
		EXPECT_GE(400, Pos.x);
		EXPECT_LE(300, Pos.y);
		EXPECT_GE(400, Pos.y);
	}
}