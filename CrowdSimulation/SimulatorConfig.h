#pragma once
#include "common/HiveConfig.h"

namespace KEY_WORDS
{
	const std::string TIMESTEP          = "TIMESTEP";
	const std::string NEIGHBOR_DIST     = "NEIGHBOR_DIST";
	const std::string MAX_NEIGHBORS     = "MAX_NEIGHBORS";
	const std::string TIME_HORIZON      = "TIME_HORIZON";
	const std::string TIME_HORIZON_OBST = "TIME_HORIZON_OBST";
	const std::string RADIUS            = "RADIUS";
	const std::string MAX_SPEED         = "MAX_SPEED";
}

class CSimulatorConfig : public hiveConfig::CHiveConfig, public hiveOO::CSingleton<CSimulatorConfig>
{
public:
	CSimulatorConfig() { __defineExtraAttribute(); }
	~CSimulatorConfig() {}

private:
	void __defineExtraAttribute()
	{
		defineAttribute(KEY_WORDS::TIMESTEP, hiveConfig::ATTRIBUTE_FLOAT);
		defineAttribute(KEY_WORDS::NEIGHBOR_DIST, hiveConfig::ATTRIBUTE_FLOAT);
		defineAttribute(KEY_WORDS::MAX_NEIGHBORS, hiveConfig::ATTRIBUTE_INT);
		defineAttribute(KEY_WORDS::TIME_HORIZON, hiveConfig::ATTRIBUTE_FLOAT);
		defineAttribute(KEY_WORDS::TIME_HORIZON_OBST, hiveConfig::ATTRIBUTE_FLOAT);
		defineAttribute(KEY_WORDS::RADIUS, hiveConfig::ATTRIBUTE_FLOAT);
		defineAttribute(KEY_WORDS::MAX_SPEED, hiveConfig::ATTRIBUTE_FLOAT);
	}

	friend class hiveOO::CSingleton<CSimulatorConfig>;
};