#pragma once
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "GLM/glm.hpp"
#include "CrowdRenderingExport.h"
#include "common/Singleton.h"

namespace hiveCrowdRendering
{
	class  CSceneVisualization : public hiveOO::CSingleton<CSceneVisualization>
	{
	public:
		~CSceneVisualization();
		void init(const std::string& vConfig);
		void drawAgent(const glm::vec2& vPos);
		void drawObstacle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom);
		void drawNode(const glm::vec2& vPos);
		void drawEdge(const glm::vec2& vNode1, const glm::vec2& vNode2);
		void display();
		void pause();
		void clear();
		void saveImage(const char* vPath);
		void saveVideo();

	private:
		CSceneVisualization() {}

		void __initScene();
		void __parseConfig(const std::string& vConfig);

	private:
		int m_Width = 0;
		int m_Height = 0;
		int m_AgentRadius = 0;
		int m_Scale = 1;
		std::tuple<int,int,int> m_BgColor;
		std::tuple<int,int,int> m_AgentColor;
		std::tuple<int, int, int> m_ObstacleColor;
		cv::Mat m_Scene;
		cv::VideoWriter m_Writer;
		std::string m_OutputDir;

		friend class hiveOO::CSingleton<CSceneVisualization>;
	};
}