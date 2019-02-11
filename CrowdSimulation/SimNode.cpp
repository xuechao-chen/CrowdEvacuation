#include "SimNode.h"

bool __isEvacuationTimeNearEqual(const std::vector<int>& vEvacuationTimeSet)
{
	//TODO 计算疏散时间是否近似相等
	return false;
}

bool CSimNode::isConverged()
{
	for (auto& AgentGroup : m_AgentGroups)
	{
		std::vector<int> EvacuationTimeSet;
		for (auto& AgentsInDiffDirection : AgentGroup)
		{
			int TotalEvacuationTime = 0;
			for (auto Agent : AgentsInDiffDirection)
			{
				TotalEvacuationTime += Agent->getEvacuationTime();
			}
			EvacuationTimeSet.push_back(TotalEvacuationTime / AgentsInDiffDirection.size());
		}
		if (!__isEvacuationTimeNearEqual(EvacuationTimeSet)) return false;
	}
	return true;
}
