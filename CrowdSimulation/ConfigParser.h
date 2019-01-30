#pragma once
#include "SceneGraph.h"
#include "../RVO/RVOSimulator.h"

class CConfigParser
{
public:
	CConfigParser();
	~CConfigParser();

	static void parseGraph(const std::string& vConfig, CSceneGraph* voGraph);
	static void parseRVOSimulator(const std::string& vConfig, RVO::RVOSimulator* voSimulator);
	//static void parseAgents(const std::string& vConfig, );
};