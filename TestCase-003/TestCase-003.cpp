#include <iostream>
#include "../CrowdSimulation/ShortestStrategy.h"
#include "../CrowdSimulation/SceneVis.h"

int main()
{
	CShortestStrategy* pStrategy = new CShortestStrategy();
	pStrategy->run();
	return 0;
}