#include "pch.h"
#include <stdarg.h>
#include "../CrowdSimulation/SceneGraph.h"
#include "common/HiveCommonMicro.h"

class TestSceneGraph : public ::testing::Test
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

	void validEdgeWeight(double vExceptedVal, const glm::vec2& vFromNode, const glm::vec2& vToNode)
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

	void validAdjNodeSet(const glm::vec2& vFromNode, int vExpectedNum, ...)
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
	std::vector<glm::vec2> m_NodeSet = { {30,30}, {210,30}, {30,210}, {210,210}, {270,210} };
};

TEST_F(TestSceneGraph, DefaultInitialization)
{
	validNumOfNodeAndEdge(0, 0);
}

TEST_F(TestSceneGraph, InitializationWithPreConstruction)
{
	constructGraph();
	validNumOfNodeAndEdge(5, 5);
}

TEST_F(TestSceneGraph, ClearGraph)
{
	constructGraph();

	m_pGraph->clearGraph();
	validNumOfNodeAndEdge(0, 0);
}

TEST_F(TestSceneGraph, DumpAdjNodeSet)
{
	constructGraph();
	auto& AdjNodeSet = m_pGraph->dumpAdjNodeSet(m_NodeSet[0]);

	ASSERT_EQ(2, AdjNodeSet.size());
	EXPECT_EQ(m_NodeSet[1], AdjNodeSet[0].first);
	EXPECT_EQ(m_NodeSet[2], AdjNodeSet[1].first);
}

TEST_F(TestSceneGraph, AddNode)
{
	constructGraph();

	m_pGraph->addNode({6,6});
	validNumOfNodeAndEdge(6, 5);

	m_pGraph->addNode(glm::vec2(7, 7));
	validNumOfNodeAndEdge(7, 5);
}

TEST_F(TestSceneGraph, RemoveNode)
{
	constructGraph();

	m_pGraph->removeNode(m_NodeSet[4]);
	validNumOfNodeAndEdge(4, 4);

	m_pGraph->removeNode(m_NodeSet[0]);
	validNumOfNodeAndEdge(3, 2);
}

TEST_F(TestSceneGraph, AddEdge)
{
	constructGraph();

	m_pGraph->addEdge(m_NodeSet[1], m_NodeSet[2], 10);
	validNumOfNodeAndEdge(5, 6);

	auto& AllEdges = m_pGraph->dumpAllEdges();
	ASSERT_EQ(6, AllEdges.size());
	EXPECT_EQ(m_NodeSet[1], AllEdges[5].first);
	EXPECT_EQ(m_NodeSet[2], AllEdges[5].second);
}

TEST_F(TestSceneGraph, RemoveEdge)
{
	constructGraph();

	m_pGraph->removeEdge(m_NodeSet[0], m_NodeSet[1]);
	validNumOfNodeAndEdge(5, 4);

	m_pGraph->removeEdge(m_NodeSet[0], m_NodeSet[2]);
	validNumOfNodeAndEdge(5, 3);

	auto& AllEdges = m_pGraph->dumpAllEdges();
	ASSERT_EQ(3, AllEdges.size());
}

TEST_F(TestSceneGraph, UpdateEdgeWeight)
{
	constructGraph();

	m_pGraph->updateEdgeWeight(m_NodeSet[0], m_NodeSet[1], 10);
	validEdgeWeight(10, m_NodeSet[0], m_NodeSet[1]);

	m_pGraph->updateEdgeWeight(m_NodeSet[0], m_NodeSet[1], 5);
	validEdgeWeight(5, m_NodeSet[0], m_NodeSet[1]);
}

TEST_F(TestSceneGraph, FindShortestPath)
{
	constructGraph();

	auto Path = m_pGraph->findShortestPath(m_NodeSet[0], m_NodeSet[4]);
	validShortestPath(Path, 4, m_NodeSet[0], m_NodeSet[1], m_NodeSet[3], m_NodeSet[4]);
}

TEST_F(TestSceneGraph, DumpNavNodes)
{
	constructGraph();

	glm::vec2 Location1(25, 25);
	auto NavNodes1 = m_pGraph->dumpNavNodes(Location1);
	ASSERT_EQ(1, NavNodes1.size());
	EXPECT_EQ(m_NodeSet[0], NavNodes1[0]);

	glm::vec2 Location2(100, 25);
	auto NavNodes2 = m_pGraph->dumpNavNodes(Location2);
	ASSERT_EQ(2, NavNodes2.size());
	EXPECT_EQ(m_NodeSet[0], NavNodes2[0]);
	EXPECT_EQ(m_NodeSet[1], NavNodes2[1]);
}