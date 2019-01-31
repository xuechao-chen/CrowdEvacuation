#pragma once
#include "common/HiveConfig.h"

namespace KEY_WORDS
{
	const std::string OBSTACLE = "OBSTACLE";
	const std::string EXIT = "EXIT";
	const std::string OBSTACLE_NUM = "OBSTACLE_NUM";
	const std::string EXIT_NUM = "EXIT_NUM";
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
	}

	friend class hiveOO::CSingleton<CSceneConfig>;
};