#pragma once
#include "BaseStrategy.h"
#include "BaseAgent.h"

class CShortestStrategy : public IEvacuationStrategy
{
public:
	CShortestStrategy();
	~CShortestStrategy();

private:
	void __init();
	bool __isFinish() override;
	void __onPreDoStep() override;
	void __onPostDoStep() override;

	void __constructNavNodeMap();
	void __add2NavNodeMap(const std::vector<glm::vec2>& vShortestPath);
	void __assignNavNode2Agent();

private:
	std::unordered_map<glm::vec2, glm::vec2, HashFunc4Node> m_NavNodeMap;
};