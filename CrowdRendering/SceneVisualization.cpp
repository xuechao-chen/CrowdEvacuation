#include "SceneVisualization.h"
#include "CrowdRenderingConfig.h"
#include "common/ConfigInterface.h"
#include "opencv2/highgui/highgui.hpp"

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
    auto CenterPos = cv::Point(vPos.x*m_Scale, vPos.y*m_Scale);
	int r, g, b;
	std::tie(r, g, b) = m_AgentColor;
	cv::circle(m_Scene, CenterPos, m_AgentRadius*m_Scale, cv::Scalar(r, g, b), CV_FILLED);
}

void CSceneVisualization::drawObstacle(const glm::vec2& vLeftTop, const glm::vec2& vRightBottom)
{
	auto w = abs(vRightBottom.x - vLeftTop.x);
	auto h = abs(vRightBottom.y - vLeftTop.y);
	auto Rect = cv::Rect(vLeftTop.x*m_Scale, vLeftTop.y*m_Scale, w*m_Scale, h*m_Scale);
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

	auto Rect = cv::Rect(0, 0, m_Width*m_Scale, m_Height*m_Scale);

	cv::rectangle(m_Scene, Rect, cv::Scalar(0,0,0), m_Scale*3);
	for (auto& Exit : m_Exits)
	{
		auto Point1 = cv::Point((Exit.x - 30)*m_Scale, (Exit.y + 30)*m_Scale);
		auto Point2 = cv::Point((Exit.x + 30)*m_Scale, (Exit.y + 30)*m_Scale);
		cv::line(m_Scene, Point1, Point2, cv::Scalar(255,255,255),m_Scale*3);
	}
	
	auto IsDisplay = CCrowdRenderingConfig::getInstance()->getAttribute<bool>(KEY_WORDS::DISPLAY);
	if (!IsDisplay) return;

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
	m_Scene = cv::Mat(m_Height*m_Scale, m_Width*m_Scale, CV_8UC3, cv::Scalar(r, g, b));
}

void CSceneVisualization::saveImage(const char* vPath)
{
	auto IsSave = CCrowdRenderingConfig::getInstance()->getAttribute<bool>(KEY_WORDS::SAVE);
	if (!IsSave) return;

	cv::imwrite((m_OutputDir + vPath).data(), m_Scene);
}

void CSceneVisualization::saveVideo()
{
	auto IsSave = CCrowdRenderingConfig::getInstance()->getAttribute<bool>(KEY_WORDS::SAVE);
	if (!IsSave) return;

	m_Writer << m_Scene;
}

void CSceneVisualization::__parseConfig(const std::string& vConfig)
{
	hiveConfig::hiveParseConfig(vConfig, hiveConfig::EConfigType::XML, CCrowdRenderingConfig::getInstance());

	auto NumExits = CCrowdRenderingConfig::getInstance()->getAttribute<int>(KEY_WORDS::EXIT_NUM);
	std::string ExitSetStr = CCrowdRenderingConfig::getInstance()->getAttribute<std::string>(KEY_WORDS::EXIT);

	int* ExitSetPos = new int[NumExits * 2];
	hiveCommon::hiveSplitLine2IntArray(ExitSetStr, " ", NumExits * 2, ExitSetPos);
	for (size_t i = 0; i < NumExits; i++)
	{
		m_Exits.push_back({ ExitSetPos[2 * i], ExitSetPos[2 * i + 1] });
	}

	m_Scale = CCrowdRenderingConfig::getInstance()->getAttribute<int>(KEY_WORDS::SCALE);

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

	m_OutputDir = CCrowdRenderingConfig::getInstance()->getAttribute<std::string>(KEY_WORDS::OUTPUT_DIR);

	m_Writer.open(m_OutputDir+"demo.avi", CV_FOURCC('M', 'J', 'P', 'G'), 60.0, cv::Size(m_Width*m_Scale, m_Height*m_Scale));
}

void CSceneVisualization::__initScene()
{
	int r, g, b;
	std::tie(r, g, b) = m_BgColor;
	m_Scene = cv::Mat(m_Height*m_Scale, m_Width*m_Scale, CV_8UC3, cv::Scalar(r, g, b));
}