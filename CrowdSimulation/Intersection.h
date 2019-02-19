#pragma once
#include <vector>
#include <unordered_map>
#include "GLM/glm.hpp"
#include <boost/math/distributions/normal.hpp>

class SAgentArriveMoment
{
public:
	SAgentArriveMoment(float vArriveMoment, float vProbability) : 
		m_NormalDist(vArriveMoment, float(log(vArriveMoment + 1)) / 3.0f),
		m_Probability(vProbability) {}

	float arriveMoment() { return m_NormalDist.mean(); }
	float cdf(float vValue) { return boost::math::cdf(m_NormalDist, vValue); }

private:
	float m_Probability;
	boost::math::normal_distribution<> m_NormalDist;
};

class CIntersection
{
public:
	CIntersection(const glm::vec2& vPosition) : m_Position(vPosition), m_Sorted(false) {}
	~CIntersection() {}

	void clear();
	float calCDF(float vArriveTime);
	glm::vec2 getPosition() const { return m_Position; }
	void addArriveMoment(float vArriveTime, float vProbability);

private:
	bool m_Sorted;
	std::vector<SAgentArriveMoment*> m_TimeLine;
	glm::vec2 m_Position;
};