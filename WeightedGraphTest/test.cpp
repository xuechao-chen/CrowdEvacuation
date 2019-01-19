#include "pch.h"
#include <stdarg.h>
#include "../CrowdSimulation/WeightedGraph.h"

class TestWeightedGraph : public testing::Test
{
protected:
	virtual void SetUp() override
	{
		pGraph = new CWeightedGraph();
	}
	virtual void TearDown() override
	{
		delete pGraph;
		pGraph = nullptr;
	}

	void constructIsolatedGraph()
	{
		pGraph->initGraph(5);
	}
	void constructConnectedGraph()
	{
		pGraph->addNode(1);
		pGraph->addNode(3);
		pGraph->addNode(5);
		pGraph->addNode(7);
		pGraph->addNode(9);
		
		pGraph->addEdge(1, 3, 5);
		pGraph->addEdge(1, 5, 5);
		pGraph->addEdge(1, 7, 5);
		pGraph->addEdge(5, 7, 5);
	}
	void validAdjNodeSet(int vFromNodeIdx, int vExpectedNum, ... )
	{
		auto& AdjNodeSet = pGraph->dumpAdjNodeSet(vFromNodeIdx);
		ASSERT_EQ(vExpectedNum, AdjNodeSet.size());

		va_list args;
		va_start(args, vExpectedNum);
		for (auto i = 0; i < vExpectedNum; i++)
		{
			auto NodeIdx = va_arg(args, int);
			EXPECT_EQ(NodeIdx, AdjNodeSet[i].first);
		}
		va_end(args);
	}
	void validNumOfNodeAndEdge(int vNumNodes, int vNumEdges)
	{
		EXPECT_EQ(vNumNodes, pGraph->getNumNodes());
		EXPECT_EQ(vNumEdges, pGraph->getNumEdges());
	}
	void validShortestPath(std::stack<int>& vPath, int vExpectedNum, ...)
	{
		ASSERT_EQ(vExpectedNum, vPath.size());

		va_list args;
		va_start(args, vExpectedNum);
		for (auto i = 0; i < vExpectedNum; i++)
		{
			auto NodeIdx = va_arg(args, int);
			EXPECT_EQ(NodeIdx, vPath.top());
			vPath.pop();
		}
		va_end(args);
	}

	CWeightedGraph* pGraph = nullptr;
};

TEST_F(TestWeightedGraph, DefaultInitialization)
{
	validNumOfNodeAndEdge(0, 0);
}

TEST_F(TestWeightedGraph, InitializationWithIsolatedGraph)
{
	constructIsolatedGraph();

	validNumOfNodeAndEdge(5, 0);
}

TEST_F(TestWeightedGraph, InitializationWithConnectedGraph)
{
	constructConnectedGraph();

	validNumOfNodeAndEdge(5, 4);

	validAdjNodeSet(1, 3, 3, 5, 7);
	validAdjNodeSet(3, 1, 1);
	validAdjNodeSet(5, 2, 1, 7);
	validAdjNodeSet(7, 2, 1, 5);
	validAdjNodeSet(9, 0);
}

TEST_F(TestWeightedGraph, DumpAdjNodeSet)
{
	constructConnectedGraph();

	auto& AdjNodeSet = pGraph->dumpAdjNodeSet(1);
	ASSERT_EQ(3, AdjNodeSet.size());
	EXPECT_EQ(3, AdjNodeSet[0].first);
	EXPECT_EQ(5, AdjNodeSet[1].first);
	EXPECT_EQ(7, AdjNodeSet[2].first);
}

TEST_F(TestWeightedGraph, AddNodeWithIsolatedGraph)
{
	constructIsolatedGraph();

	pGraph->addNode(5);
	validNumOfNodeAndEdge(6, 0);

	pGraph->addNode(6);
	validNumOfNodeAndEdge(7, 0);
}

TEST_F(TestWeightedGraph, AddNodeWithConnectedGraph)
{
	constructConnectedGraph();

	pGraph->addNode(2);
	validNumOfNodeAndEdge(6, 4);

	pGraph->addNode(4);
	validNumOfNodeAndEdge(7, 4);
}

TEST_F(TestWeightedGraph, RemoveNodeWithIsolatedGraph)
{
	constructIsolatedGraph();

	pGraph->removeNode(0);
	validNumOfNodeAndEdge(4, 0);

	pGraph->removeNode(1);
	validNumOfNodeAndEdge(3, 0);
}

TEST_F(TestWeightedGraph, RemoveNodeWithConnectedGraph)
{
	constructConnectedGraph();

	pGraph->removeNode(3);
	validNumOfNodeAndEdge(4, 3);

	pGraph->removeNode(5);
	validNumOfNodeAndEdge(3, 1);
}

TEST_F(TestWeightedGraph, AddEdgeWithIsolatedGraph)
{
	constructIsolatedGraph();

	pGraph->addEdge(0, 3, 5);
	validNumOfNodeAndEdge(5, 1);

	pGraph->addEdge(2, 4, 5);
	validNumOfNodeAndEdge(5, 2);
}

TEST_F(TestWeightedGraph, AddEdgeWithConnectedGraph)
{
	constructConnectedGraph();

	pGraph->addEdge(7, 9, 5);
	validNumOfNodeAndEdge(5, 5);
	validAdjNodeSet(9, 1, 7);
}

TEST_F(TestWeightedGraph, RemoveEdge)
{
	constructConnectedGraph();

	pGraph->removeEdge(1, 3);
	validNumOfNodeAndEdge(5, 3);
	validAdjNodeSet(1, 2, 5, 7);
	validAdjNodeSet(3, 0);

	pGraph->removeEdge(1, 7);
	validNumOfNodeAndEdge(5, 2);
	validAdjNodeSet(1, 1, 5);
	validAdjNodeSet(7, 1, 5);
}

TEST_F(TestWeightedGraph, UpdateEdgeWeight)
{
	constructConnectedGraph();

	pGraph->updateEdgeWeight(1, 3, 10);
	EXPECT_EQ(10, pGraph->getEdgeWeight(1, 3));
	EXPECT_EQ(10, pGraph->getEdgeWeight(3, 1));

	pGraph->updateEdgeWeight(1, 5, 2.8);
	EXPECT_EQ(2.8, pGraph->getEdgeWeight(1, 5));
	EXPECT_EQ(2.8, pGraph->getEdgeWeight(5, 1));
}

TEST_F(TestWeightedGraph, FindShortestPath)
{
	constructConnectedGraph();
	pGraph->addEdge(3, 5, 3);
	pGraph->addEdge(7, 9, 5);

	auto& Path1 = pGraph->findShortestPath(1, 9);
	validShortestPath(Path1, 3, 1, 7, 9);

	auto& Path2 = pGraph->findShortestPath(3, 9);
	validShortestPath(Path2, 4, 3, 5, 7, 9);
}
