#pragma once
#include "BaseStrategy.h"
#include "BaseAgent.h"

class CShortestStrategy : public IEvacuationStrategy
{
public:
	CShortestStrategy();
	~CShortestStrategy();

private:
	bool __isFinish() override;
	void __afterSimulationDoStep() override;
	void __constructRoadMap() override;
	void __assignNavNode2Agent() override;

	void __add2RoadMap(const std::vector<glm::vec2>& vShortestPath);

private:
	std::unordered_map<glm::vec2, glm::vec2, HashFunc4Node> m_RoadMap;
};