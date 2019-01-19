#pragma once
#include "BaseEvacuationStrategy.h"

using Path = std::vector<glm::vec2>;

class CSimulationStrategy : public IEvacuationStrategy
{
public:
	CSimulationStrategy();
	~CSimulationStrategy();

	void run() override;

private:
	void __initPaths();
	void __updatePaths();
	
private:
	int m_IterationTime;
	std::unordered_map<int, Path> m_Paths;
};

