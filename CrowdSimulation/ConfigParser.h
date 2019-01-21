#pragma once
#include "common/Singleton.h"
#include "WeightedGraph.h"

class CConfigParser : public hiveOO::CSingleton<CConfigParser>
{
public:
	CConfigParser();
	~CConfigParser();

	void parseGraph(const std::string& vConfig, std::unordered_map<Node, AdjNodePosSet, HashFunc4Vec2>& vNodePosMap);

private:
	friend class hiveOO::CSingleton<CConfigParser>;

};