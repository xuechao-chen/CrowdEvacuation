#pragma once
#include "BaseEvacuationStrategy.h"

class CCDFPredictionStrategy : public IEvacuationStrategy
{
public:
	CCDFPredictionStrategy();
	~CCDFPredictionStrategy();

	void run() override;
};

