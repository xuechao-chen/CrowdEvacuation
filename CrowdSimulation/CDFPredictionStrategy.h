#pragma once
#include "BaseStrategy.h"
#include "Intersection.h"

class CCDFPredictionStrategy : public IEvacuationStrategy
{
public:
	CCDFPredictionStrategy();
	~CCDFPredictionStrategy();

private:
	void init() override;
	bool __isFinish() override;
	void __afterSimulationDoStep() override;
	void __constructRoadMap() override;
	void __assignNavNode2Agent() override;

	void __add2RoadMap(const std::vector<glm::vec2>& vShortestPath);
	void __initIntersections();
	void __updateIntersections();
	void __updateAgentsNavigation();

private:
	std::unordered_map<glm::vec2, glm::vec2, HashFunc4Node> m_RoadMap;
	std::unordered_map<glm::vec2, CIntersection*, HashFunc4Node> m_Intersections;
};

float __CDF2Factor(float vCDF);

/*
- 计算当前时刻路口的CDF
- 计算未来某个时刻路口的CDF
- 根据边上人的移动速度计算边的权重
- 计算两点之间最短路径的通行时间
- 计算边的人群平均移动速度
- 将CDF映射成系数
- 计算Agent在未来某个时刻的位置
*/

