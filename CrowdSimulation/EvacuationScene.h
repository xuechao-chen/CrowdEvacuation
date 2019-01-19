#pragma once
#include <vector>
#include "GLM/glm.hpp"
#include "GraphAdapter.h"

// 管理疏散场景的通行区域信息，可通行区域的连接结构图
class CEvacuationScene
{
public:
	CEvacuationScene();
	CEvacuationScene(int vWidth, int vHeight) : m_Width(vWidth), m_Height(vHeight) {}
	~CEvacuationScene();

	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }
	void setWidth(int vWidth) { m_Width = vWidth; }
	void setHeight(int vHeight) { m_Height = vHeight; }

	std::vector<std::pair<glm::vec2, glm::vec2>> getObstacles() const { return m_Obstacles; }
	void addObstacle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom);
	void removeObstacle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom);
	
	void setGraphAdapter(CGraphAdapter* pAdapter) { m_pGraphAdapter = pAdapter; }
	void addNode(const glm::vec2& vPos);
	void addEdge(const glm::vec2& vPos1, const glm::vec2& vPos2);
	void removeNode(const glm::vec2& vPos);
	void removeEdge(const glm::vec2& vPos1, const glm::vec2& vPos2);
	double getEdgeWeight(const glm::vec2& vPos1, const glm::vec2& vPos2) const;
	void updateEdgeWeight(const glm::vec2& vPos1, const glm::vec2& vPos2);
	std::stack<glm::vec2> findShortestPath(const glm::vec2& vFromPos, const glm::vec2& vToPos) const;

private:
	int m_Width;
	int m_Height;
	std::vector<std::pair<glm::vec2, glm::vec2>> m_Obstacles;
	CGraphAdapter* m_pGraphAdapter = nullptr;
};

