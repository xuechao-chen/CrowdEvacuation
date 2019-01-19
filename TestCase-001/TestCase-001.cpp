#include <iostream>
#include "../CrowdRendering/CrowdRenderingInterface.h"

int main(int, char*[])
{
	hiveCrowdRendering::init("RenderingConfig.xml");

	std::pair<glm::vec2, glm::vec2> ObstaclePos = std::make_pair(glm::vec2(10, 10), glm::vec2(50, 50));
	std::vector<std::pair<glm::vec2, glm::vec2>> Obstacles;
	Obstacles.push_back(ObstaclePos);

	std::vector<glm::vec2> Agents;
	for (auto i = 0; i < 100; i++)
	{
		glm::vec2 AgentPos(i + 10, 100);
		Agents.clear();
		Agents.push_back(AgentPos);
		hiveCrowdRendering::draw(Obstacles, Agents);
	}

	hiveCrowdRendering::pause();

	return 0;
}