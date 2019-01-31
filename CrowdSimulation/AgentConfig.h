#pragma once
#include "common/HiveConfig.h"

namespace KEY_WORDS
{
	const std::string GROUP  = "GROUP";
	const std::string REGION = "REGION";
	const std::string AMOUNT = "AMOUNT";
}

class CAgentConfig : public hiveConfig::CHiveConfig, public hiveOO::CSingleton<CAgentConfig>
{
public:
	CAgentConfig() { __defineExtraAttribute(); }
	~CAgentConfig() {}

private:
	void __defineExtraAttribute()
	{
		defineAttribute(KEY_WORDS::GROUP,  hiveConfig::ATTRIBUTE_SUBCONFIG);
		defineAttribute(KEY_WORDS::REGION, hiveConfig::ATTRIBUTE_STRING);
		defineAttribute(KEY_WORDS::AMOUNT, hiveConfig::ATTRIBUTE_INT);
	}

	friend class hiveOO::CSingleton<CAgentConfig>;
};