#pragma once
#include "common/HiveConfig.h"

namespace KEY_WORDS
{
	const std::string NODE = "NODE";
	const std::string EDGE = "EDGE";
	const std::string NODE_NUM = "NODE_NUM";
	const std::string EDGE_NUM = "EDGE_NUM";
}

class CGraphConfig : public hiveConfig::CHiveConfig, public hiveOO::CSingleton<CGraphConfig>
{
public:
	CGraphConfig() { __defineExtraAttribute(); }
	~CGraphConfig() {}
	
private:
	void __defineExtraAttribute()
	{
		defineAttribute(KEY_WORDS::NODE, hiveConfig::ATTRIBUTE_STRING);
		defineAttribute(KEY_WORDS::EDGE, hiveConfig::ATTRIBUTE_STRING);
		defineAttribute(KEY_WORDS::NODE_NUM, hiveConfig::ATTRIBUTE_INT);
		defineAttribute(KEY_WORDS::EDGE_NUM, hiveConfig::ATTRIBUTE_INT);
	}

	friend class hiveOO::CSingleton<CGraphConfig>;
};