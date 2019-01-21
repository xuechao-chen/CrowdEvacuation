#include "pch.h"
#include <stdarg.h>
#include "../CrowdSimulation/WeightedGraph.h"
#include "common/HiveCommonMicro.h"

class TestGraph :public testing::Test
{
protected:
	virtual void SetUp() override
	{
		pGraph = new CWeightedGraph();
	}
	virtual void TearDown() override
	{
		_SAFE_DELETE(pGraph);
	}

	void constructGraph()
	{
		pGraph->constructGraph("Config.xml");
	}

	void validNumOfNodeAndEdge(int vNumNodes, int vNumEdges)
	{
		EXPECT_EQ(vNumNodes, pGraph->getNumNodes());
		EXPECT_EQ(vNumEdges, pGraph->getNumEdges());
	}

	void validGetEdgeWeight(double vExceptedNum, glm::vec2 vFromNodePos, glm::vec2 vToNodePos)
	{
		EXPECT_NEAR(vExceptedNum, pGraph->getEdgeWeight(vFromNodePos, vToNodePos), 0.0001);
	}

	void validShortestPath(std::vector<glm::vec2>& vPath, int vExpectedNum, ...)
	{
		ASSERT_EQ(vExpectedNum, vPath.size());

		va_list args;
		va_start(args, vExpectedNum);
		for (auto i = 0; i < vExpectedNum; i++)
		{
			auto NodePos = va_arg(args, glm::vec2);
			EXPECT_EQ(NodePos, vPath[i]);
		}
		va_end(args);
	}

	void validAdjNodeSet(glm::vec2 vFromNodePos, int vExpectedNum, ...)
	{
		auto AdjNodePosSet = pGraph->dumpAdjNodeSet(vFromNodePos);
		ASSERT_EQ(vExpectedNum, AdjNodePosSet.size());

		va_list args;
		va_start(args, vExpectedNum);
		for (auto i = 0; i < vExpectedNum; i++)
		{
			auto NodePos = va_arg(args, glm::vec2);
			EXPECT_EQ(NodePos, AdjNodePosSet[i].first);
		}
		va_end(args);
	}

	CWeightedGraph* pGraph = nullptr;
};

TEST_F(TestGraph, defaultInitiallization)
{
	validNumOfNodeAndEdge(0, 0);
}

TEST_F(TestGraph, initiallization)
{
	constructGraph();
	validNumOfNodeAndEdge(6, 7);
	validAdjNodeSet(glm::vec2(1, 1), 3, glm::vec2(4, 1), glm::vec2(7, 1), glm::vec2(1, 3));
	validGetEdgeWeight(3, glm::vec2(1, 1), glm::vec2(4, 1));
	validGetEdgeWeight(6, glm::vec2(1, 1), glm::vec2(7, 1));
	validGetEdgeWeight(2, glm::vec2(1, 1), glm::vec2(1, 3));
	validGetEdgeWeight(sqrt(13), glm::vec2(7, 1), glm::vec2(4, 3));
}

TEST_F(TestGraph, DumpAdjNodePosSet)
{
	constructGraph();
	auto& AdjNodePosSet = pGraph->dumpAdjNodeSet(glm::vec2(1, 1));
	ASSERT_EQ(3, AdjNodePosSet.size());
	EXPECT_EQ(glm::vec2(4, 1), AdjNodePosSet[0].first);
	EXPECT_EQ(glm::vec2(7, 1), AdjNodePosSet[1].first);
	EXPECT_EQ(glm::vec2(1, 3), AdjNodePosSet[2].first);
}

TEST_F(TestGraph, AddNode)
{
	constructGraph();
	validNumOfNodeAndEdge(6, 7);
	pGraph->addNode(glm::vec2(1, 1));
	validNumOfNodeAndEdge(6, 7);

	pGraph->addNode(glm::vec2(8, 3));
	validNumOfNodeAndEdge(7, 7);
	auto& AdjNodePosSet = pGraph->dumpAdjNodeSet(glm::vec2(8, 3));
	ASSERT_EQ(0, AdjNodePosSet.size());
}

TEST_F(TestGraph, RemoveNode)
{
	constructGraph();
	validNumOfNodeAndEdge(6, 7);
	pGraph->removeNode(glm::vec2(1, 1));
	validNumOfNodeAndEdge(5, 4);
	auto& AdjNodePosSet = pGraph->dumpAdjNodeSet(glm::vec2(1, 1));
	EXPECT_EQ(0, AdjNodePosSet.size());
	AdjNodePosSet = pGraph->dumpAdjNodeSet(glm::vec2(4, 1));
	EXPECT_EQ(1, AdjNodePosSet.size());

	pGraph->removeNode(glm::vec2(8, 3));
	validNumOfNodeAndEdge(5, 4);
}

TEST_F(TestGraph, AddEdge)
{
	constructGraph();
	validNumOfNodeAndEdge(6, 7);
	pGraph->addEdge(glm::vec2(4, 1), glm::vec2(7, 1), 5);
	validNumOfNodeAndEdge(6, 8);
	pGraph->addEdge(glm::vec2(4, 1), glm::vec2(7, 1), 5);
	validNumOfNodeAndEdge(6, 8);
}

TEST_F(TestGraph, RemoveEdge)
{
	constructGraph();
	validNumOfNodeAndEdge(6, 7);

	pGraph->removeEdge(glm::vec2(1, 1), glm::vec2(4, 1));
	validNumOfNodeAndEdge(6, 6);
	validAdjNodeSet(glm::vec2(1, 1), 2, glm::vec2(7, 1), glm::vec2(1, 3));

	pGraph->removeEdge(glm::vec2(1, 1), glm::vec2(4, 1));
	validNumOfNodeAndEdge(6, 6);
	validAdjNodeSet(glm::vec2(1, 1), 2, glm::vec2(7, 1), glm::vec2(1, 3));
}

TEST_F(TestGraph, UpdateEdgeWeight)
{
	constructGraph();

	pGraph->updateEdgeWeight(glm::vec2(1, 1), glm::vec2(4, 1), 10);
	validGetEdgeWeight(10, glm::vec2(1, 1), glm::vec2(4, 1));
	pGraph->updateEdgeWeight(glm::vec2(1, 1), glm::vec2(4, 1), -1);
	validGetEdgeWeight(10, glm::vec2(1, 1), glm::vec2(4, 1));

	pGraph->updateEdgeWeight(glm::vec2(1, 2), glm::vec2(4, 1), 10);
	validGetEdgeWeight(0, glm::vec2(1, 2), glm::vec2(4, 1));
}

TEST_F(TestGraph, FindShortestPath)
{
	constructGraph();

	auto Path1 = pGraph->findShortestPath(glm::vec2(1, 1), glm::vec2(7, 3));
	validShortestPath(Path1, 4, glm::vec2(1, 1), glm::vec2(1, 3), glm::vec2(4, 3), glm::vec2(7, 3));

	auto Path2 = pGraph->findShortestPath(glm::vec2(4, 1), glm::vec2(4, 3));
	validShortestPath(Path2, 3, glm::vec2(4, 1), glm::vec2(1, 3), glm::vec2(4, 3));
}