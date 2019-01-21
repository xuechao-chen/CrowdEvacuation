#pragma once
#include "common/HiveConfig.h"

namespace KEY_WORDS
{
	const std::string NODE = "Node";
	const std::string NODE_NUM = "NodeNum";
	const std::string NODE_ITEM = "NodeItem";
	const std::string EDGE = "Edge";
	const std::string EDGE_NUM = "EdgeNum";
	const std::string EDGE_ITEM = "EdgeItem";
}

class CGraphConfig :public hiveConfig::CHiveConfig, public hiveOO::CSingleton<CGraphConfig>
{
public:
	CGraphConfig();
	~CGraphConfig();
	void defineExtraAttribute(int vNodeNum = 1, int vEdgeNum = 1);
private:
	void __defineBaseAttribute();

	friend class hiveOO::CSingleton<CGraphConfig>;
};