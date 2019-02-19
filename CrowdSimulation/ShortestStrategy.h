#pragma once
#include "BaseStrategy.h"

class CShortestStrategy : public IEvacuationStrategy
{
public:
	CShortestStrategy() = default;
	~CShortestStrategy() = default;

private:
	void __afterSimulationDoStep() override;
};