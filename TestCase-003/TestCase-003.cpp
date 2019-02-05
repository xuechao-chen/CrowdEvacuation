#include <iostream>
#include "../CrowdSimulation/ShortestStrategy.h"
#include "../CrowdSimulation/SceneVis.h"

int main()
{
	IEvacuationStrategy* pStrategy = new CShortestStrategy();
	pStrategy->init();
	pStrategy->run();
	return 0;
}