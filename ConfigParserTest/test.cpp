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

TEST(TestConfigParser, ParseAgent)
{

}