﻿#include "../CrowdSimulation/CDFPredictionStrategy.h"

int main()
{
	IEvacuationStrategy* pStrategy = new CCDFPredictionStrategy();
	pStrategy->init();
	pStrategy->run();
	return 0;
}