#include "Intersection.h"

//const float NUM_A = pow(2.0f, 1.0f / 50.0f);

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
	auto NeighborSize = 5.0f;
	for (auto ArriveMoment : m_TimeLine)
	{
		auto Integral = ArriveMoment->cdf(vArriveTime + NeighborSize) - ArriveMoment->cdf(vArriveTime - NeighborSize);
		CDF += Integral;
	}
	return CDF;
}