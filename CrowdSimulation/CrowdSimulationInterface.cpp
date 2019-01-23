#include "CrowdSimulationInterface.h"
#include "ShortestStrategy.h"
#include "common/HiveCommonMicro.h"

void simulation(const std::string & vConfig)
{
	IEvacuationStrategy* pStrategy = new CShortestStrategy();
	pStrategy->run();
	
	_SAFE_DELETE(pStrategy);
}