#pragma once
#include "common/HiveConfig.h"

namespace KEY_WORDS
{
	const std::string STRATEGY            = "STRATEGY";
	const std::string SHORTEST_STRATEGY   = "SHORTEST_STRATEGY";
	const std::string SIMULATION_STRATEGY = "SIMULATION_STRATEGY";
	const std::string CDF_STRATEGY		  = "CDF_STRATEGY";
}

class CStrategyConfig : public hiveConfig::CHiveConfig, public hiveOO::CSingleton<CStrategyConfig>
{
public:
	CStrategyConfig() { __defineExtraAttribute(); }
	~CStrategyConfig() {}

private:
	void __defineExtraAttribute()
	{
		defineAttribute(KEY_WORDS::STRATEGY,            hiveConfig::ATTRIBUTE_STRING);
		defineAttribute(KEY_WORDS::SHORTEST_STRATEGY,   hiveConfig::ATTRIBUTE_STRING);
		defineAttribute(KEY_WORDS::SIMULATION_STRATEGY, hiveConfig::ATTRIBUTE_STRING);
		defineAttribute(KEY_WORDS::CDF_STRATEGY,        hiveConfig::ATTRIBUTE_STRING);
	}

	friend class hiveOO::CSingleton<CStrategyConfig>;
};