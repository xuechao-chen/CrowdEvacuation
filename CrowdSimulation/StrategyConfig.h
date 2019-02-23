#pragma once
#include "common/HiveConfig.h"

namespace KEY_WORDS
{
	const std::string STRATEGY            = "STRATEGY";
	const std::string SHORTEST_STRATEGY   = "SHORTEST_STRATEGY";
	const std::string SIMULATION_STRATEGY = "SIMULATION_STRATEGY";
	const std::string CDF_STRATEGY		  = "CDF_STRATEGY";

	//CDF params
	const std::string MAX_FACTOR		  = "MAX_FACTOR";
	const std::string NEIGHBOR_SIZE		  = "NEIGHBOR_SIZE";
	const std::string MAX_AGENT			  = "MAX_AGENT";
	const std::string UPDATE_DENSITY	  = "UPDATE_DENSITY";
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
		defineAttribute(KEY_WORDS::MAX_FACTOR,          hiveConfig::ATTRIBUTE_FLOAT);
		defineAttribute(KEY_WORDS::NEIGHBOR_SIZE,       hiveConfig::ATTRIBUTE_FLOAT);
		defineAttribute(KEY_WORDS::MAX_AGENT,			hiveConfig::ATTRIBUTE_FLOAT);
		defineAttribute(KEY_WORDS::UPDATE_DENSITY,		hiveConfig::ATTRIBUTE_BOOL);
	}

	friend class hiveOO::CSingleton<CStrategyConfig>;
};