#include <iostream>
#include "../CrowdSimulation/ShortestStrategy.h"

int main()
{
	IEvacuationStrategy* pStrategy = new CShortestStrategy();
	pStrategy->init();
	pStrategy->run();
	return 0;
}