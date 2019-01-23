#include "ConfigParser.h"
#include "GraphConfig.h"
#include "common/ConfigInterface.h"

CConfigParser::CConfigParser()
{
}

CConfigParser::~CConfigParser()
{
}

void CConfigParser::parseGraph(const std::string & vConfig, std::unordered_map<Node, AdjNodeSet, HashFunc4Node>& vNodePosMap)
{
	auto pAdaptConfig = CGraphConfig::getInstance();

	hiveConfig::hiveParseConfig(vConfig, hiveConfig::EConfigType::XML, pAdaptConfig);
	int nodeNum = pAdaptConfig->getAttribute<int>("NodeNum");
	int edgeNum = pAdaptConfig->getAttribute<int>("EdgeNum");

	pAdaptConfig->defineExtraAttribute(nodeNum, edgeNum);
	hiveConfig::hiveParseConfig(vConfig, hiveConfig::EConfigType::XML, pAdaptConfig);
	auto pNodeConfig = pAdaptConfig->findSubConfigByName("node");
	auto pEdegeConfig = pAdaptConfig->findSubConfigByName("edge");
	std::unordered_map<int, glm::vec2> idx2PosMap;
	for (int i = 0; i < nodeNum; i++)
	{
		boost::tuple<int, int> nodePos = pNodeConfig->getAttribute<boost::tuple<int, int>>("NodeItem" + std::to_string(i));
		idx2PosMap[i] = glm::vec2(boost::get<0>(nodePos), boost::get<1>(nodePos));
	}
	std::vector<boost::tuple<int, int>> tmpEdgeVec;
	for (int i = 0; i < edgeNum; i++)
	{
		boost::tuple<int, int> edgePos = pEdegeConfig->getAttribute<boost::tuple<int, int>>("EdgeItem" + std::to_string(i));
		tmpEdgeVec.push_back(edgePos);
	}
	for (int i = 0; i < nodeNum; i++)
	{
		AdjNodeSet tmpNodeSet;
		for (auto it = tmpEdgeVec.cbegin(); it != tmpEdgeVec.cend(); it++)
		{
			if (boost::get<0>(*it) == i || boost::get<1>(*it) == i)
			{
				glm::vec2 fromPos = idx2PosMap[i];
				glm::vec2 toPos;
				if (boost::get<0>(*it) == i)
					toPos = idx2PosMap[boost::get<1>(*it)];
				else
				{
					toPos = idx2PosMap[boost::get<0>(*it)];
				}
				double tmpWeight = sqrt(pow((fromPos.x - toPos.x), 2) + pow((fromPos.y - toPos.y), 2));
				std::pair<Node, EdgeWeight> tmpPair = std::make_pair(toPos, tmpWeight);
				tmpNodeSet.push_back(tmpPair);
			}
		}
		vNodePosMap[idx2PosMap[i]] = tmpNodeSet;
	}
}


