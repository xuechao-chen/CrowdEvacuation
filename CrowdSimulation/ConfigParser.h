#pragma once
#include "common/Singleton.h"
#include "SceneGraph.h"

class CConfigParser
{
public:
	CConfigParser();
	~CConfigParser();

	static void parseGraph(const std::string& vConfig, CSceneGraph* voGraph);
	static void parseAgents();
};