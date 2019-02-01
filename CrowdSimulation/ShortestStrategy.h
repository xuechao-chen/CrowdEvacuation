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
};

