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

TEST_F(TestSimulationStrategy, AddDivideNode2RoadMap)
{
	m_pStrategy->__addDivideNode2RoadMap();
	auto RoadMap = m_pStrategy->m_RoadMap;
	ASSERT_EQ(7, RoadMap.size());

	auto SimNode1 = RoadMap[glm::vec2(1, 1)];
	auto SimNode2 = RoadMap[glm::vec2(4, 4)];
	ASSERT_EQ(2, SimNode1->getNavNodeNum());
	ASSERT_EQ(2, SimNode2->getNavNodeNum());
	EXPECT_EQ(ESimNodeType::DivideNode, SimNode1->getNodeType());
	EXPECT_EQ(ESimNodeType::DivideNode, SimNode2->getNodeType());

	EXPECT_EQ(glm::vec2(2,2), SimNode1->getNavNodeAt(0));
	EXPECT_EQ(glm::vec2(3,3), SimNode1->getNavNodeAt(1));
	EXPECT_EQ(glm::vec2(5,5), SimNode2->getNavNodeAt(0));
	EXPECT_EQ(glm::vec2(6,6), SimNode2->getNavNodeAt(1));
}

TEST_F(TestSimulationStrategy, AddDistributionNode2RoadMap)
{
	m_pStrategy->__addDistributionNode2RoadMap();
	auto RoadMap = m_pStrategy->m_RoadMap;
	ASSERT_EQ(5, RoadMap.size());

	auto SimNode = RoadMap[glm::vec2(30, 30)];
	ASSERT_EQ(2, SimNode->getNavNodeNum());
	EXPECT_EQ(ESimNodeType::DistributionNode, SimNode->getNodeType());

	EXPECT_EQ(glm::vec2(30, 150), SimNode->getNavNodeAt(0));
	EXPECT_EQ(glm::vec2(150, 30), SimNode->getNavNodeAt(1));
	EXPECT_FLOAT_EQ(0.3, SimNode->getDistributionRatioAt(0));
	EXPECT_FLOAT_EQ(0.7, SimNode->getDistributionRatioAt(1));
}