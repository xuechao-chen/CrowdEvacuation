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
	CGraphConfig();
	~CGraphConfig() {}
	
private:
	void __defineExtraAttribute();

	friend class hiveOO::CSingleton<CGraphConfig>;
};