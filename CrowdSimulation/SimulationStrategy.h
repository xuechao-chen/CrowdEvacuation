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

	void __initPathSet();
	void __updatePathSet();
	
private:
	int m_IterationTime;
	std::vector<Path> m_PathSet;
};

