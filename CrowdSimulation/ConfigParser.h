#pragma once
#include "common/Singleton.h"
#include "SceneGraph.h"

class CConfigParser : public hiveOO::CSingleton<CConfigParser>
{
public:
	CConfigParser();
	~CConfigParser();

	void parseGraph(const std::string& vConfig, std::unordered_map<Node, AdjNodeSet, HashFunc4Node>& vNodePosMap);

private:
	friend class hiveOO::CSingleton<CConfigParser>;

};