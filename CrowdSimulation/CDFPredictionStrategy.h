#pragma once
#include "BaseStrategy.h"
#include "Intersection.h"

class CCDFPredictionStrategy : public IEvacuationStrategy
{
public:
	CCDFPredictionStrategy() = default;
	~CCDFPredictionStrategy() = default;

private:
	void init() override;
	void __afterSimulationDoStep() override;

	void __initIntersections();
	void __updateIntersections();
	void __updateAgentsNavigation();
	void __updateEdgeWeight();

private:
	std::unordered_map<glm::vec2, CIntersection*, HashFunc4Node> m_Intersections;
};

float __CDF2Factor(float vCDF);