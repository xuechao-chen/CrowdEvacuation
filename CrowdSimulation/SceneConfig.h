#pragma once
#include "common/HiveConfig.h"

namespace KEY_WORDS
{
	const std::string OBSTACLE = "OBSTACLE";
	const std::string EXIT = "EXIT";
	const std::string OBSTACLE_NUM = "OBSTACLE_NUM";
	const std::string EXIT_NUM = "EXIT_NUM";

	const std::string GROUP = "GROUP";
	const std::string REGION = "REGION";
	const std::string AMOUNT = "AMOUNT";
}

class CSceneConfig : public hiveConfig::CHiveConfig, public hiveOO::CSingleton<CSceneConfig>
{
public:
	CSceneConfig() { __defineExtraAttribute(); }
	~CSceneConfig() {}

private:
	void __defineExtraAttribute()
	{
		defineAttribute(KEY_WORDS::OBSTACLE, hiveConfig::ATTRIBUTE_STRING);
		defineAttribute(KEY_WORDS::EXIT, hiveConfig::ATTRIBUTE_STRING);
		defineAttribute(KEY_WORDS::OBSTACLE_NUM, hiveConfig::ATTRIBUTE_INT);
		defineAttribute(KEY_WORDS::EXIT_NUM, hiveConfig::ATTRIBUTE_INT);

		defineAttribute(KEY_WORDS::GROUP, hiveConfig::ATTRIBUTE_SUBCONFIG);
		defineAttribute(KEY_WORDS::REGION, hiveConfig::ATTRIBUTE_STRING);
		defineAttribute(KEY_WORDS::AMOUNT, hiveConfig::ATTRIBUTE_INT);
	}

	friend class hiveOO::CSingleton<CSceneConfig>;
};