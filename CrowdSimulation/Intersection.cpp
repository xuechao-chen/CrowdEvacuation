#include "Intersection.h"
#include "StrategyConfig.h"

bool AgentArriveMoment_comp(SAgentArriveMoment* a, SAgentArriveMoment* b) 
{
	return (a->arriveMoment() < b->arriveMoment());
}

void CIntersection::addArriveMoment(float vArriveTime, float vProbability)
{
	SAgentArriveMoment* pArriveMoment = new SAgentArriveMoment(vArriveTime, vProbability);
	m_TimeLine.push_back(pArriveMoment);
}

void CIntersection::clear()
{
	m_TimeLine.clear();
}

float CIntersection::calCDF(float vArriveTime)
{
	auto CDF = 0.0f;
	auto NeighborSize = CStrategyConfig::getInstance()->getAttribute<float>(KEY_WORDS::NEIGHBOR_SIZE);
	for (auto ArriveMoment : m_TimeLine)
	{
		auto Integral = ArriveMoment->cdf(vArriveTime + NeighborSize*3) - ArriveMoment->cdf(vArriveTime - NeighborSize*3);
		CDF += Integral;
	}
	return CDF;
}