#pragma once
#include <vector>
#include "GLM/glm.hpp"
#include "GraphAdapter.h"

// 管理疏散场景的通行区域信息(出口/障碍物)，可通行区域的连接结构图
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
	
	std::vector<glm::vec2> getExits() const { return m_Exits; }
	void addExit(const glm::vec2& vExit);
	void removeExit(const glm::vec2& vExit);

	void setGraphAdapter(CGraphAdapter* pAdapter) { m_pGraphAdapter = pAdapter; }
	CGraphAdapter* getGraphAdapter() const { return m_pGraphAdapter; }

private:
	int m_Width;
	int m_Height;
	std::vector<std::pair<glm::vec2, glm::vec2>> m_Obstacles;
	std::vector<glm::vec2> m_Exits;
	CGraphAdapter* m_pGraphAdapter = nullptr;
};

