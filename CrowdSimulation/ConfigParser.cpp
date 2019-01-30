#include "ConfigParser.h"
#include "GraphConfig.h"
#include "common/ConfigInterface.h"

CConfigParser::CConfigParser()
{
}

CConfigParser::~CConfigParser()
{
}

void CConfigParser::parseGraph(const std::string & vConfig, CSceneGraph * voGraph)
{
	bool isParsed = hiveConfig::hiveParseConfig(vConfig, hiveConfig::EConfigType::XML, CGraphConfig::getInstance());

	auto NumNode = CGraphConfig::getInstance()->getAttribute<int>(KEY_WORDS::NODE_NUM);
	auto NumEdge = CGraphConfig::getInstance()->getAttribute<int>(KEY_WORDS::EDGE_NUM);

	std::string NodeSetStr = CGraphConfig::getInstance()->getAttribute<std::string>(KEY_WORDS::NODE);
	std::string EdgeSetStr = CGraphConfig::getInstance()->getAttribute<std::string>(KEY_WORDS::EDGE);

	int* NodeSetPos = new int[NumNode * 2];
	hiveCommon::hiveSplitLine2IntArray(NodeSetStr, " ", NumNode*2, NodeSetPos);
	for (size_t i = 0; i < NumNode; i++)
	{
		voGraph->addNode({NodeSetPos[2*i], NodeSetPos[2*i+1]});
	}

	int* EdgeSetPos = new int[NumEdge * 4];
	hiveCommon::hiveSplitLine2IntArray(EdgeSetStr, " ", NumEdge*4, EdgeSetPos);
	for (size_t i = 0; i < NumEdge; i++)
	{
		glm::vec2 Node1 = { EdgeSetPos[4*i], EdgeSetPos[4*i+1] };
		glm::vec2 Node2 = { EdgeSetPos[4*i+2], EdgeSetPos[4*i+3] };
		double Weight = glm::distance(Node1, Node2);
		voGraph->addEdge(Node1, Node2, Weight);
	}
}


