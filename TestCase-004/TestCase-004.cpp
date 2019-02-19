#include "../CrowdSimulation/SimulationStrategy.h"

int main()
{
	IEvacuationStrategy* pStrategy = new CSimulationStrategy();
	pStrategy->init();
	pStrategy->run();
	return 0;
}