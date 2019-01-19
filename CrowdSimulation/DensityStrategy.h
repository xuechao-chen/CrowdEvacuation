#pragma once
#include "BaseEvacuationStrategy.h"

class CDensityStrategy : public IEvacuationStrategy
{
public:
	CDensityStrategy();
	~CDensityStrategy();

	void run() override;
};

