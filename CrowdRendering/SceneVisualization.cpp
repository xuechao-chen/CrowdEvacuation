#include "SceneVisualization.h"
#include "CrowdRenderingConfig.h"
#include "common/ConfigInterface.h"
#include "opencv2/highgui//highgui.hpp"

using namespace hiveCrowdRendering;

CSceneVisualization::~CSceneVisualization()
{
}

void CSceneVisualization::init(const std::string& vConfig)
{
	__parseConfig(vConfig);
	__initScene();
}

void CSceneVisualization::drawAgent(const glm::vec2& vPos)
{
    auto CenterPos = cv::Point(vPos.x, vPos.y);
	int r, g, b;
	std::tie(r, g, b) = m_AgentColor;
	cv::circle(m_Scene, CenterPos, m_AgentRadius, cv::Scalar(r, g, b), CV_FILLED);
}

void CSceneVisualization::drawObstacle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom)
{
	auto w = abs(vRightBottom.x - vLeftTop.x);
	auto h = abs(vRightBottom.y - vLeftTop.y);
	auto Rect = cv::Rect(vLeftTop.x, vLeftTop.y, w, h);
	int r, g, b; 
	std::tie(r, g, b) = m_ObstacleColor;
	cv::rectangle(m_Scene, Rect, cv::Scalar(r, g, b), CV_FILLED);
}

void hiveCrowdRendering::CSceneVisualization::drawNode(const glm::vec2 & vPos)
{
	auto CenterPos = cv::Point(vPos.x, vPos.y);
	int r, g, b;
	std::tie(r, g, b) = m_AgentColor;
	cv::circle(m_Scene, CenterPos, m_AgentRadius, cv::Scalar(r, g, b));
}

void hiveCrowdRendering::CSceneVisualization::drawEdge(const glm::vec2 & vNode1, const glm::vec2 & vNode2)
{
	auto Point1 = cv::Point(vNode1.x, vNode1.y);
	auto Point2 = cv::Point(vNode2.x, vNode2.y);
	int r, g, b;
	std::tie(r, g, b) = m_AgentColor;
	cv::line(m_Scene, Point1, Point2, cv::Scalar(r,g,b));
}

void CSceneVisualization::display()
{
	cv::imshow("Scene", m_Scene);
	cv::waitKey(1);
}

void CSceneVisualization::pause()
{
	cvWaitKey(0);
}

void hiveCrowdRendering::CSceneVisualization::clear()
{
	int r, g, b;
	std::tie(r, g, b) = m_BgColor;
	m_Scene = cv::Mat(m_Width, m_Height, CV_8SC3, cv::Scalar(r, g, b));
}

void CSceneVisualization::__parseConfig(const std::string& vConfig)
{
	hiveConfig::hiveParseConfig(vConfig, hiveConfig::EConfigType::XML, CCrowdRenderingConfig::getInstance());

	m_Width = CCrowdRenderingConfig::getInstance()->getAttribute<int>(KEY_WORDS::WIDTH_OF_SCENE);
	m_Height = CCrowdRenderingConfig::getInstance()->getAttribute<int>(KEY_WORDS::HEIGHT_OF_SCENE);
	m_AgentRadius = CCrowdRenderingConfig::getInstance()->getAttribute<int>(KEY_WORDS::RADIUS_OF_AGENT);

	auto BgColorStr = CCrowdRenderingConfig::getInstance()->getAttribute<std::string>(KEY_WORDS::BG_COLOR_OF_SCENE);
	int BgColor[3];
	hiveCommon::hiveSplitLine2IntArray(BgColorStr, " ", 3, BgColor);
	m_BgColor = std::make_tuple(BgColor[0], BgColor[1], BgColor[2]);

	auto AgentColorStr = CCrowdRenderingConfig::getInstance()->getAttribute<std::string>(KEY_WORDS::COLOR_OF_AGENT);
	int AgentColor[3];
	hiveCommon::hiveSplitLine2IntArray(AgentColorStr, " ", 3, AgentColor);
	m_AgentColor = std::make_tuple(AgentColor[0], AgentColor[1], AgentColor[2]);

	auto ObstacleColorStr = CCrowdRenderingConfig::getInstance()->getAttribute<std::string>(KEY_WORDS::COLOR_OF_OBSTACLE);
	int ObstacleColor[3];
	hiveCommon::hiveSplitLine2IntArray(ObstacleColorStr, " ", 3, ObstacleColor);
	m_ObstacleColor = std::make_tuple(ObstacleColor[0], ObstacleColor[1], ObstacleColor[2]);
}

void CSceneVisualization::__initScene()
{
	int r, g, b;
	std::tie(r, g, b) = m_BgColor;
	m_Scene = cv::Mat(m_Width, m_Height, CV_8SC3, cv::Scalar(r, g, b));
}