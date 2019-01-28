#pragma once
#include "BaseStrategy.h"

class CDensityStrategy : public IEvacuationStrategy
{
public:
	CDensityStrategy();
	~CDensityStrategy();

	void run() override;
};

