#include "pch.h"

class TestSimulationStrategy : public ::testing::Test
{
protected:
	virtual void SetUp() override
	{
		m_pStrategy = new CSimulationStrategy();
		m_pStrategy->init();
	}

	virtual void TearDown() override
	{
		delete m_pStrategy;
	}

	CSimulationStrategy* m_pStrategy = nullptr;
	std::vector<glm::vec2> m_NodeSet = { {30,30},{150,30},{30,150},{150,150} };
};

TEST_F(TestSimulationStrategy, ConstructRoadMap)
{
	auto& NavNodeMap = m_pStrategy->m_RoadMap;
	ASSERT_EQ(5, NavNodeMap.size());
	EXPECT_EQ(m_NodeSet[2], NavNodeMap[m_NodeSet[0]]->getNavNodeAt(0));
	EXPECT_EQ(m_NodeSet[3], NavNodeMap[m_NodeSet[1]]->getNavNodeAt(0));
	EXPECT_EQ(m_NodeSet[3], NavNodeMap[m_NodeSet[2]]->getNavNodeAt(0));
	EXPECT_EQ(glm::vec2(FLT_MAX, FLT_MAX), NavNodeMap[m_NodeSet[3]]->getNavNodeAt(0));

	auto& DivideNode = glm::vec2(90, 30);
	auto& NavNodeSet = NavNodeMap[DivideNode]->getNavNodeSet();
	ASSERT_EQ(2, NavNodeSet.size());
	EXPECT_EQ(m_NodeSet[0], NavNodeSet[0]);
	EXPECT_EQ(m_NodeSet[1], NavNodeSet[1]);

	auto Graph = m_pStrategy->getEvacuationScene()->getGraph();
	EXPECT_EQ(5, Graph->getNumNodes());
	EXPECT_EQ(5, Graph->getNumEdges());
}

TEST_F(TestSimulationStrategy, AssignNavNode2Agent)
{
	auto Scene = m_pStrategy->getEvacuationScene();
	auto& Agents = Scene->getAgents();
	ASSERT_EQ(4, Agents.size());
	EXPECT_EQ(m_NodeSet[2], Agents[0]->getNavNode());
	EXPECT_EQ(m_NodeSet[2], Agents[1]->getNavNode());
	EXPECT_EQ(m_NodeSet[0], Agents[2]->getNavNode());
	EXPECT_EQ(m_NodeSet[1], Agents[3]->getNavNode());
}