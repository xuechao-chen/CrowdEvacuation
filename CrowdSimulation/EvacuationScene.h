#pragma once
#include <vector>
#include "GLM/glm.hpp"

// 管理疏散场景的通行区域信息，可通行区域的连接结构
class CEvacuationScene
{
public:
	CEvacuationScene();
	CEvacuationScene(int vWidth, int vHeight) : m_Width(vWidth), m_Height(vHeight) {}
	~CEvacuationScene();

	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }
	void addObstacle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom);
	void removeObstacle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom);
	

private:
	int m_Width;
	int m_Height;
	std::vector<std::pair<glm::vec2, glm::vec2>> m_Obstacles;
};

