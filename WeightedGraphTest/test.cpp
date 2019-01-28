#include "pch.h"
#include <stdarg.h>
#include "../CrowdSimulation/SceneGraph.h"
#include "common/HiveCommonMicro.h"

class TestGraph :public testing::Test
{
protected:
	virtual void SetUp() override
	{
		m_pGraph = new CSceneGraph();
	}
	virtual void TearDown() override
	{
		_SAFE_DELETE(m_pGraph);
	}

	void constructGraph()
	{
		for (auto& Node : m_NodeSet)
		{
			m_pGraph->addNode(Node);
		}
		m_pGraph->addEdge(m_NodeSet[0], m_NodeSet[1], 5);
		m_pGraph->addEdge(m_NodeSet[0], m_NodeSet[2], 10);
		m_pGraph->addEdge(m_NodeSet[1], m_NodeSet[3], 10);
		m_pGraph->addEdge(m_NodeSet[2], m_NodeSet[3], 10);
		m_pGraph->addEdge(m_NodeSet[3], m_NodeSet[4], 10);
	}

	void validNumOfNodeAndEdge(int vNumNodes, int vNumEdges)
	{
		EXPECT_EQ(vNumNodes, m_pGraph->getNumNodes());
		EXPECT_EQ(vNumEdges, m_pGraph->getNumEdges());
	}

	void validEdgeWeight(double vExceptedVal, glm::vec2 vFromNode, glm::vec2 vToNode)
	{
		EXPECT_FLOAT_EQ(vExceptedVal, m_pGraph->getEdgeWeight(vFromNode, vToNode));
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

	void validAdjNodeSet(glm::vec2 vFromNode, int vExpectedNum, ...)
	{
		auto AdjNodeSet = m_pGraph->dumpAdjNodeSet(vFromNode);
		ASSERT_EQ(vExpectedNum, AdjNodeSet.size());

		va_list args;
		va_start(args, vExpectedNum);
		for (auto i = 0; i < vExpectedNum; i++)
		{
			auto Node = va_arg(args, glm::vec2);
			EXPECT_EQ(Node, AdjNodeSet[i].first);
		}
		va_end(args);
	}

	CSceneGraph* m_pGraph = nullptr;
	std::vector<glm::vec2> m_NodeSet = { {1,1}, {2,2}, {3,3}, {4,4}, {5,5} };
};

TEST_F(TestGraph, DefaultInitialization)
{
	validNumOfNodeAndEdge(0, 0);
}

TEST_F(TestGraph, InitializationWithPreConstruction)
{
	constructGraph();
	validNumOfNodeAndEdge(5, 5);
}

TEST_F(TestGraph, DumpAdjNodeSet)
{
	constructGraph();
	auto& AdjNodeSet = m_pGraph->dumpAdjNodeSet(m_NodeSet[0]);

	ASSERT_EQ(2, AdjNodeSet.size());
	EXPECT_EQ(m_NodeSet[1], AdjNodeSet[0].first);
	EXPECT_EQ(m_NodeSet[2], AdjNodeSet[1].first);
}

TEST_F(TestGraph, AddNode)
{
	constructGraph();

	m_pGraph->addNode({6,6});
	validNumOfNodeAndEdge(6, 5);

	m_pGraph->addNode(glm::vec2(7, 7));
	validNumOfNodeAndEdge(7, 5);
}

TEST_F(TestGraph, RemoveNode)
{
	constructGraph();

	m_pGraph->removeNode(m_NodeSet[4]);
	validNumOfNodeAndEdge(4, 4);

	m_pGraph->removeNode(m_NodeSet[0]);
	validNumOfNodeAndEdge(3, 2);
}

TEST_F(TestGraph, AddEdge)
{
	constructGraph();

	m_pGraph->addEdge(m_NodeSet[1], m_NodeSet[2], 10);
	validNumOfNodeAndEdge(5, 6);
}

TEST_F(TestGraph, RemoveEdge)
{
	constructGraph();

	m_pGraph->removeEdge(m_NodeSet[0], m_NodeSet[1]);
	validNumOfNodeAndEdge(5, 4);

	m_pGraph->removeEdge(m_NodeSet[0], m_NodeSet[2]);
	validNumOfNodeAndEdge(5, 3);
}

TEST_F(TestGraph, UpdateEdgeWeight)
{
	constructGraph();

	m_pGraph->updateEdgeWeight(m_NodeSet[0], m_NodeSet[1], 10);
	validEdgeWeight(10, m_NodeSet[0], m_NodeSet[1]);

	m_pGraph->updateEdgeWeight(m_NodeSet[0], m_NodeSet[1], 5);
	validEdgeWeight(5, m_NodeSet[0], m_NodeSet[1]);
}

TEST_F(TestGraph, FindShortestPath)
{
	constructGraph();

	auto Path = m_pGraph->findShortestPath(m_NodeSet[0], m_NodeSet[4]);
	validShortestPath(Path, 4, m_NodeSet[0], m_NodeSet[1], m_NodeSet[3], m_NodeSet[4]);
}