#pragma once
#include "BaseStrategy.h"
#include "BaseAgent.h"

class CShortestStrategy : public IEvacuationStrategy
{
public:
	CShortestStrategy();
	~CShortestStrategy();

	void run() override;

private:
	std::vector<IAgent*> m_Agent;
};

