#include "GraphConfig.h"

CGraphConfig::CGraphConfig()
{
	__defineExtraAttribute();
}

void CGraphConfig::__defineExtraAttribute()
{
	defineAttribute(KEY_WORDS::NODE, hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(KEY_WORDS::EDGE, hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(KEY_WORDS::NODE_NUM, hiveConfig::ATTRIBUTE_INT);
	defineAttribute(KEY_WORDS::EDGE_NUM, hiveConfig::ATTRIBUTE_INT);
}