#include <iostream>
#include "../CrowdRendering/CrowdRenderingInterface.h"

using leftTop = glm::vec2;
using rightBottom = glm::vec2;
using agentPosition = glm::vec2;
using obsPosition = std::pair<leftTop, rightBottom>;
namespace obstaclePosSet
{
	const static obsPosition obstaclePos1 = std::make_pair(glm::vec2(40, 40), glm::vec2(230, 230));
	const static obsPosition obstaclePos2 = std::make_pair(glm::vec2(270, 40), glm::vec2(460, 230));
	const static obsPosition obstaclePos3 = std::make_pair(glm::vec2(40, 270), glm::vec2(230, 460));
	const static obsPosition obstaclePos4 = std::make_pair(glm::vec2(270, 270), glm::vec2(460, 460));
}
namespace agentPosSet
{
	const static agentPosition agentPos1 = glm::vec2(20, 20);
	const static agentPosition agentPos2 = glm::vec2(20, 135);
	const static agentPosition agentPos3 = glm::vec2(20, 250);
	const static agentPosition agentPos4 = glm::vec2(20, 365);
	const static agentPosition agentPos5 = glm::vec2(20, 480);
	const static agentPosition agentPos6 = glm::vec2(250, 20);
	const static agentPosition agentPos7 = glm::vec2(250, 135);
	const static agentPosition agentPos8 = glm::vec2(250, 250);
	const static agentPosition agentPos9 = glm::vec2(250, 365);
	const static agentPosition agentPos10 = glm::vec2(250, 480);
	const static agentPosition agentPos11 = glm::vec2(480, 20);
	const static agentPosition agentPos12 = glm::vec2(480, 135);
	const static agentPosition agentPos13 = glm::vec2(480, 250);
	const static agentPosition agentPos14 = glm::vec2(480, 365);
	const static agentPosition agentPos15 = glm::vec2(480, 480);
}

void initialPosVec(std::vector<obsPosition>& vObstaclePosVec, std::vector<glm::vec2>& vAgentsPosVec);

int main(int, char*[])
{
	hiveCrowdRendering::init("RenderingConfig.xml");

	std::vector<obsPosition> ObstaclesPosVec;
	std::vector<glm::vec2> AgentsPosVec;
	initialPosVec(ObstaclesPosVec, AgentsPosVec);

	hiveCrowdRendering::drawObstacles(ObstaclesPosVec);
	hiveCrowdRendering::drawAgents(AgentsPosVec);
	hiveCrowdRendering::display();
	hiveCrowdRendering::pause();

	return 0;
}

void initialPosVec(std::vector<obsPosition>& vObstaclePosVec, std::vector<glm::vec2>& vAgentsPosVec)
{
	vObstaclePosVec.push_back(obstaclePosSet::obstaclePos1);
	vObstaclePosVec.push_back(obstaclePosSet::obstaclePos2);
	vObstaclePosVec.push_back(obstaclePosSet::obstaclePos3);
	vObstaclePosVec.push_back(obstaclePosSet::obstaclePos4);
	vAgentsPosVec.push_back(agentPosSet::agentPos1);
	vAgentsPosVec.push_back(agentPosSet::agentPos2);
	vAgentsPosVec.push_back(agentPosSet::agentPos3);
	vAgentsPosVec.push_back(agentPosSet::agentPos4);
	vAgentsPosVec.push_back(agentPosSet::agentPos5);
	vAgentsPosVec.push_back(agentPosSet::agentPos6);
	vAgentsPosVec.push_back(agentPosSet::agentPos7);
	vAgentsPosVec.push_back(agentPosSet::agentPos8);
	vAgentsPosVec.push_back(agentPosSet::agentPos9);
	vAgentsPosVec.push_back(agentPosSet::agentPos10);
	vAgentsPosVec.push_back(agentPosSet::agentPos11);
	vAgentsPosVec.push_back(agentPosSet::agentPos12);
	vAgentsPosVec.push_back(agentPosSet::agentPos13);
	vAgentsPosVec.push_back(agentPosSet::agentPos14);
	vAgentsPosVec.push_back(agentPosSet::agentPos15);
}