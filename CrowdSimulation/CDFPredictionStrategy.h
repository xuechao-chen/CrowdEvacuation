#pragma once
#include "BaseStrategy.h"

class CCDFPredictionStrategy : public IEvacuationStrategy
{
public:
	CCDFPredictionStrategy();
	~CCDFPredictionStrategy();

	void run() override;
};

