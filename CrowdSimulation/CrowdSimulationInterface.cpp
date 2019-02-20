#include "CrowdSimulationInterface.h"
#include "common/ConfigInterface.h"
#include "StrategyConfig.h"
#include "BaseStrategy.h"
#include "common/ProductFactoryData.h"

void simulation(const std::string & vConfig)
{
	hiveConfig::hiveParseConfig(vConfig, hiveConfig::EConfigType::XML, CStrategyConfig::getInstance());
	auto StrategySig = CStrategyConfig::getInstance()->getAttribute<std::string>(KEY_WORDS::STRATEGY);
	
	IEvacuationStrategy* pStrategy = dynamic_cast<IEvacuationStrategy*>(hiveOO::CProductFactoryData::getInstance()->createProduct(StrategySig));
	pStrategy->init();
	pStrategy->run();
	delete pStrategy;
}