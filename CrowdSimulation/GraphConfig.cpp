#include "GraphConfig.h"

CGraphConfig::CGraphConfig( )
{
	__defineBaseAttribute();
}

CGraphConfig::~CGraphConfig()
{
}

void CGraphConfig::defineExtraAttribute(int vNodeNum , int vEdgeNum )
{
	defineAttribute(KEY_WORDS::NODE, hiveConfig::ATTRIBUTE_SUBCONFIG);
	defineAttribute(KEY_WORDS::EDGE, hiveConfig::ATTRIBUTE_SUBCONFIG);
	for (int i = 0; i < vNodeNum; i++)
	{
		defineAttribute(KEY_WORDS::NODE_ITEM+std::to_string(i), hiveConfig::ATTRIBUTE_VEC2I);
	}
	for (int i = 0; i < vEdgeNum; i++)
	{
		defineAttribute(KEY_WORDS::EDGE_ITEM + std::to_string(i), hiveConfig::ATTRIBUTE_VEC2I);
	}
}

void CGraphConfig::__defineBaseAttribute()
{
	defineAttribute(KEY_WORDS::NODE_NUM, hiveConfig::ATTRIBUTE_INT);
	defineAttribute(KEY_WORDS::EDGE_NUM, hiveConfig::ATTRIBUTE_INT);
}
