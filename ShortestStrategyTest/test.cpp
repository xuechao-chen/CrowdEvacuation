#include "pch.h"
#include "../CrowdSimulation/SceneVis.h"

class TestShortestStrategy : public ::testing::Test
{
protected:
	virtual void SetUp() override
	{
		m_pStrategy = new CShortestStrategy();
	}

	virtual void TearDown() override
	{
		delete m_pStrategy;
	}

	CShortestStrategy* m_pStrategy = nullptr;
	std::vector<glm::vec2> m_NodeSet = { {30,30},{150,30},{30,150},{150,150} };
};

TEST_F(TestShortestStrategy, ConstructNavNodeMap)
{
	auto& NavNodeMap = m_pStrategy->m_RoadMap;
	ASSERT_EQ(4, NavNodeMap.size());
	EXPECT_EQ(m_NodeSet[2], NavNodeMap[m_NodeSet[0]]);
	EXPECT_EQ(m_NodeSet[3], NavNodeMap[m_NodeSet[1]]);
	EXPECT_EQ(m_NodeSet[3], NavNodeMap[m_NodeSet[2]]);
	EXPECT_EQ(glm::vec2(FLT_MAX, FLT_MAX), NavNodeMap[m_NodeSet[3]]);
}

TEST_F(TestShortestStrategy, AssignNavNode2Agent)
{
	auto Scene = m_pStrategy->getEvacuationScene();
	auto& Agents = Scene->getAgents();
	ASSERT_EQ(5, Agents.size());
	EXPECT_EQ(m_NodeSet[2], Agents[0]->getNavNode());
	EXPECT_EQ(m_NodeSet[2], Agents[1]->getNavNode());
	EXPECT_EQ(m_NodeSet[0], Agents[2]->getNavNode());
	EXPECT_EQ(m_NodeSet[1], Agents[3]->getNavNode());
	EXPECT_EQ(glm::vec2(FLT_MAX, FLT_MAX), Agents[4]->getNavNode());
}